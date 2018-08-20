      program grda_ceus

c Make 10^ai (/cell/y) grid from catalog for CEUS 
c This version: 
c  Write text output files ga.a.asc & ga.a.csv; see also grda_ceus_bin.f 
c  NSHM 2015 catalog format; count by E[M] & N* 
c  Force uniform rates for etennz & newmad zones (optional) 
c  Fixed-kernel 2-d gaussian smoothing 
c  File fort.99 is catalog of counted eqks 

c Usage: grda catalog_filename mc.in kye bv ag lz 
c   where 
c  mc.in specifies completeness for 6+1 zones (below) 
c  kye = catalog end year 
c  bv = b value 
c  ag = smoothing correlation length (km) 
c  lz = 1 to force uniform etennz & newmad rates, 0 otherwise 

c Example, Model 1 for 2014 NSHM: 
c  grda emm.c3 m.in 2012 1.0 50. 1 
c  m.in: 
c    2.7  1970  cz1 
c    2.7  1930  cz2 
c    2.7  1975  cz3 
c    2.7  1980  cz4 
c    2.7  1980  cz5 
c    2.7  1960  cz6 
c    2.7  1980  czo 
c Example, Model 3 for 2014 NSHM: 
c  grda emm.c3 m.in 2012 1.0 75. 1 
c  m.in: 
c    4.7  1800  cz1 
c    4.7  1700  cz2 
c    4.7  1800  cz3 
c    4.7  1800  cz4 
c    4.7  1850  cz5 
c    4.7  1850  cz6 
c    4.7  1850  czo 

c Written by C. Mueller, USGS (original agrid codes by A. Frankel) 
c See also: grda_yc, grdabz, adaptrky 
c References: 
c  R. Herrmann, 1977, Earthquake Notes 
c  CEUS-SSC, 2012, www.ceus-ssc.com 
c Notes: 
c  1) NSHM completeness zonation is a simplified version of CEUS-SSC 
c  2) Threshold mag can vary by zone 
c  3) Make distinct agrids for distinct b-value regions; consider how 
c     to merge historical & background rates, and trim & combine grids. 
c History: 
c  2018_04_17: This version w/ asc,csv output for 2018 NSHM 
c  2018_04_16: Update six completeness zones (z18_cmpl*) 
c              Update rates for etennz & newmad
c  2018_04_16: Reproduce results from previous code that got completeness 
c   info from command line; see results at U48e_2018_04_16_testnew. 

      parameter (nmax=499000)
      character buf*120
      logical inetennz,innewmad
      logical 
     * z18_cmpl1,z18_cmpl2,z18_cmpl3,z18_cmpl4,z18_cmpl5,z18_cmpl6
c      logical inzcmpl1,inzcmpl2,inzcmpl3,inzcmpl4,inzcmpl5,inzcmpl6
      dimension cm(7),ky(7),f(7),asum(nmax),asum2(nmax)
      dtor= atan(1.)/45.

c setup
      call getarg(1,buf) ! catalog file
      open (unit=12,file=buf)
      call getarg(2,buf) ! completeness model file
      open (unit=13,file=buf)
      call getarg(3,buf)
      read (buf,*) kye
      call getarg(4,buf)
      read (buf,*) bv
      call getarg(5,buf)
      read (buf,*) ag
      call getarg(6,buf)
      read (buf,*) lz
      do i=1,7
        read (13,*) cm(i),ky(i)
        f(i)= 10.**(bv*cm(i))/(kye-ky(i)+1)
      enddo

      open (unit=11,file='ena.gctrl')
      read (11,*) ymin,ymax
      read (11,*) xmin,xmax
      read (11,*) dsy,dsx
      ny= nint((ymax-ymin)/dsy)+1
      nx= nint((xmax-xmin)/dsx)+1
      nj= nx*ny
      write (*,*) 'nx,ny,nj=',nx,ny,nj

c zone counts & rates (/km^2/y)
c Jan2018 (~/SH_US48e/CAT/Analysis_ZoneRates;emm_pn.c3)
      retennz= 0.77e-04 ! #count,M3.0+,1972-2017
      rnewmad= 0.75e-04 ! #count,M3.0+,1972-2017
      retennz= retennz*(10.**(bv*3.0)) ! 10^ac
      rnewmad= rnewmad*(10.**(bv*3.0)) ! 10^ac
cc 2014 NSHM (~/solaris/PSHA/US_48e/2014_nshm/ZONE_RATES/ALT;emm.c4) 
c      retennz= 0.81e-04 ! #count,M3.0+,1972-2012
c      rnewmad= 0.62e-04 ! #count,M3.0+,1972-2012
c      retennz= retennz*(10.**(bv*3.0)) ! 10^ac
c      rnewmad= rnewmad*(10.**(bv*3.0)) ! 10^ac

      write (*,*) 'kye,bv,ag,lz=',kye,bv,ag,lz
      do i=1,7
        write (*,*) 'i,cm,ky,f=',i,cm(i),ky(i),f(i)
      enddo
      write (*,*) 'retennz,rnewmad=',retennz,rnewmad

c count catalog
c asum= 10^ac (/cell/y)
      do j=1,nj
        asum(j)= 0.
      enddo
1     read (12,'(a)',end=8) buf
      nbuf= len(trim(buf))
      read (buf,*) x,eln,elt,idp,iy4,i,i,i,i,x,x,xm,xn
      if (iy4.gt.kye) goto 1
      jy= nint((ymax-(elt-0.00001))/dsy)+1
      if (jy.lt.1.or.jy.gt.ny) goto 1
      jx= nint((eln+0.00001-xmin)/dsx)+1
      if (jx.lt.1.or.jx.gt.nx) goto 1
      j= (jy-1)*nx+jx
      if (z18_cmpl1(eln,elt)) then
c      if (inzcmpl1(eln,elt)) then
        if (xm.ge.cm(1).and.iy4.ge.ky(1)) then
          asum(j)= asum(j)+xn*f(1)
          goto 7
        endif
      elseif (z18_cmpl2(eln,elt)) then
c      elseif (inzcmpl2(eln,elt)) then
        if (xm.ge.cm(2).and.iy4.ge.ky(2)) then
          asum(j)= asum(j)+xn*f(2)
          goto 7
        endif
      elseif (z18_cmpl3(eln,elt)) then
c      elseif (inzcmpl3(eln,elt)) then
        if (xm.ge.cm(3).and.iy4.ge.ky(3)) then
          asum(j)= asum(j)+xn*f(3)
          goto 7
        endif
      elseif (z18_cmpl4(eln,elt)) then
c      elseif (inzcmpl4(eln,elt)) then
        if (xm.ge.cm(4).and.iy4.ge.ky(4)) then
          asum(j)= asum(j)+xn*f(4)
          goto 7
        endif
      elseif (z18_cmpl5(eln,elt)) then
c      elseif (inzcmpl5(eln,elt)) then
        if (xm.ge.cm(5).and.iy4.ge.ky(5)) then
          asum(j)= asum(j)+xn*f(5)
          goto 7
        endif
      elseif (z18_cmpl6(eln,elt)) then
c      elseif (inzcmpl6(eln,elt)) then
        if (xm.ge.cm(6).and.iy4.ge.ky(6)) then
          asum(j)= asum(j)+xn*f(6)
          goto 7
        endif
      else
        if (xm.ge.cm(7).and.iy4.ge.ky(7)) then
          asum(j)= asum(j)+xn*f(7)
          goto 7
        endif
      endif
      goto 1
7     write (99,'(a)') buf(1:nbuf)
      goto 1
8     if (lz.ne.1) goto 9
      do jy=1,ny
        y= ymax-(jy-1)*dsy
        ca= 111.1*dsy*111.1*dsx*cos(y*dtor)
        do jx=1,nx
          x= xmin+(jx-1)*dsx
          j= (jy-1)*nx+jx
          if (inetennz(x,y)) asum(j)= retennz*ca
          if (innewmad(x,y)) asum(j)= rnewmad*ca
        enddo
      enddo
9     continue

c asum= 10^ai (/cell/y)
      dmi= 0.1
      fh= (10.**(bv*dmi/2.)-10.**(-bv*dmi/2.))
      write (*,*) 'fh=',fh
      do j=1,nj
        asum(j)= fh*asum(j)
      enddo

c smooth - go out to 3*ag
      sum1= 0.
      do j=1,nj
        sum1= sum1+asum(j) 
      enddo
      write (*,'(a,e11.3)') ' sum1=',sum1
      if (ag.eq.0.) goto 29
      ag2= ag*ag
      avelat= (ymin+ymax)/2.
      lx= 3.*ag/(111.1*cos(avelat*dtor)*dsx)
      ly= 3.*ag/(111.1*dsy)
      write (*,*) 'lx,ly=',lx,ly
      do 24 jy=1,ny
        y1= ymax-(jy-1)*dsy
c        write (*,'(a,f9.3)') 'smoothing... y=',y1
        do 23 jx=1,nx
          x1= xmin+(jx-1)*dsx
          j= (jy-1)*nx+jx
          suma= 0.
          sumb= 0.
          do 22 jyy=jy-ly,jy+ly
            if (jyy.lt.1.or.jyy.gt.ny) goto 22
            y2= ymax-(jyy-1)*dsy
            do 21 jxx=jx-lx,jx+lx
              if (jxx.lt.1.or.jxx.gt.nx) goto 21
              x2= xmin+(jxx-1)*dsx
              jj= (jyy-1)*nx+jxx
              call delaz(y2,x2,y1,x1,dist,az,baz)
              dist2= dist*dist
              w= exp(-dist2/ag2)
              suma= suma+asum(jj)*w
              sumb= sumb+w
21          continue
22        continue
          asum2(j)= suma/sumb
23      continue
24    continue
29    sum2= 0.
      do j=1,nj
        sum2= sum2+asum2(j)
      enddo
      write (*,'(a,e11.3)') ' sum2=',sum2

c finish
      open (21,file='ga.a.asc')
      open (22,file='ga.a.csv')
      do jy=1,ny
        y= ymax-(jy-1)*dsy
        do jx=1,nx
          x= xmin+(jx-1)*dsx
          j= (jy-1)*nx+jx
          write (21,'(f9.3,f8.3,e12.4)') x,y,asum2(j)
          write (22,'(f9.3,a,f8.3,a,e12.4)') x,',',y,', GR,',asum2(j)
        enddo
      enddo

      stop
      end

      subroutine delaz(sorlat,sorlon,stnlat,stnlon,delta,az,baz)
      coef= atan(1.)/45.
      xlat= sorlat*coef
      xlon= sorlon*coef
      st0= cos(xlat)
      ct0= sin(xlat) 
      phi0= xlon
      xlat= stnlat*coef
      xlon= stnlon*coef
      ct1= sin(xlat)
      st1= cos(xlat)
      sdlon= sin(xlon-phi0)
      cdlon= cos(xlon-phi0)
      cdelt= st0*st1*cdlon+ct0*ct1
      x= st0*ct1-st1*ct0*cdlon
      y= st1*sdlon
      sdelt= sqrt(x*x+y*y)
      delta= atan2(sdelt,cdelt)
      delta= delta/coef
      az= atan2(y,x)
      az= az/coef
      x= st1*ct0-st0*ct1*cdlon
      y= -sdlon*st0
      baz= atan2(y,x)
      baz= baz/coef
      delta= delta*111.1
      return
      end         
