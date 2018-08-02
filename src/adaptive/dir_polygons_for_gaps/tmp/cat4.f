      program cat4

c Separate suspected fluid-induced eqks from catalog emm.c3
c  emm.c4= emm.c3 with SFI eqks deleted
c  emm.c4x= catalog of SFI eqks
c  (So emm.c4 + emm.c4x = emm.c3)

c is_ashtabula 1987-2007
c is_brewton 1997-2012
c is_cogdell 1976-2012 (zone from Ellsworth)
c is_cok 2006-2012 (broad zone encompasses 3 subzones)
c  (is_cok1: Prague/Sparks Feb2010; incl M5.6,06Nov2011)
c  (is_cok2: closer to Oklahoma City Mar2006)
c  (is_cok3: southeast of Prague Nov2009)
c is_dfw 2008-2012 (broader zone)
c is_fashing 1973-2012 (zone from Ellsworth; encompass Pleasanton)
c is_ggark 2009-2012 (zone from Ellsworth)
c is_timpson 2011-2012
c is_raton 2001-2012
c is_youngstown 2010-2012
c  is_daggr 1998-2012 (update?)
c  is_denvr 1962-1979 (update?)
c  is_prdox 1991-2012 (update?)
c  is_rngly 1957-2012 (update?)

c Written by C. Mueller, USGS.
c f77 -O -o cat4 cat4.f ~/PSHA/US_48zon/z48subs.o

      character buf*128
      logical is_ashtabula,is_brewton,is_cok,is_cogdell,is_dfw,
     * is_fashing,is_ggark,is_raton,is_timpson,is_youngstown,
     * is_daggr,is_denvr,is_prdox,is_rngly

      open (unit=11,file='emm.c3',readonly)
      open (12,file='emm.c4')
      open (13,file='emm.c4x')

      n12= 0
      n13= 0
1     read (11,'(q,a)',end=2) n,buf
      read (buf,*) emg,eln,elt,idp,iy4,icm,icd
      if (is_ashtabula(eln,elt).and.iy4.ge.1987.and.iy4.le.2007) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_brewton(eln,elt).and.iy4.ge.1997) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_cogdell(eln,elt).and.iy4.ge.1976) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_cok(eln,elt).and.iy4.ge.2006) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_dfw(eln,elt).and.iy4.ge.2008) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_fashing(eln,elt).and.iy4.ge.1973) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_ggark(eln,elt).and.iy4.ge.2009) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_timpson(eln,elt).and.iy4.ge.2011) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_raton(eln,elt).and.iy4.ge.2001) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_youngstown(eln,elt).and.iy4.ge.2010) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_daggr(eln,elt).and.iy4.ge.1998) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_denvr(eln,elt).and.iy4.ge.1962.and.iy4.le.1979) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_prdox(eln,elt).and.iy4.ge.1991) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      elseif (is_rngly(eln,elt).and.iy4.ge.1957) then
        write (13,'(a)') buf(1:n)
        n13= n13+1
      else
        write (12,'(a)') buf(1:n)
        n12= n12+1
      endif
      goto 1
2     continue

      write (*,*)
      write (*,'(a,3i7)') 'n12,n13=',n12,n13

      stop

      end
