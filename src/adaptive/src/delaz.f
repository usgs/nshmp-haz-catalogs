c --------------------------------------------------------------
      subroutine delaz(sorlat,sorlon,stnlat,stnlon,delta,az,baz)
c --------------------------------------------------------------
      coef= 3.14159/180.
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
      delta= delta*111.11

c optional printing
c     if ( delta.le.200. ) then
c        write(*,17),sorlat,sorlon,stnlat,stnlon,delta
c     end if
 17   format('delaz-lat1/lon1/lat2/lon2/delta: ',4(f7.2,', '),f7.1)

      return
      end
c --------------------------------------------------------------
