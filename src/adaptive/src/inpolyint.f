c ===========================================================================
      integer function LXYINT (X,Y,PX,PY,N)
c ---------------------------------------------------------------------------
c modified function to return integer - 1==.true.; 0==.false.
c 
c Is point (X,Y) inside the (unclosed) polygon (PX,PY)?
c See "Application of the winding-number algorithm...",
c  by Godkin and Pulli, BSSA, pp. 1845-1848, 1984.
c LXYIN= .true. if (X,Y) is inside or on polygon, .false. otherwise.
c Written by C. Mueller, USGS.
      dimension PX(N),PY(N)
      LXYINT=1
      KSUM= 0
      do 1 I=1,N-1
        K= KPSCR(PX(I)-X,PY(I)-Y,PX(I+1)-X,PY(I+1)-Y)
        if (K.eq.4) return
        KSUM= KSUM+K
1       continue
      K= KPSCR(PX(N)-X,PY(N)-Y,PX(1)-X,PY(1)-Y)
      if (K.eq.4) return
      KSUM= KSUM+K
      if (KSUM.eq.0) LXYINT=0
      return
      end
c ===========================================================================

c ===========================================================================
      integer function KPSCR (X1,Y1,X2,Y2)
c ---------------------------------------------------------------------------
c Compute the signed crossing number of the segment from (X1,Y1) to (X2,Y2).
c See "Application of the winding-number algorithm...",
c  by Godkin and Pulli, BSSA, pp. 1845-1848, 1984.
c KPSCR= +4 if segment passes through the origin
c        +2 if segment crosses -x axis from below
c        +1 if segment ends on -x axis from below or starts up from -x axis
c         0 if no crossing
c        -1 if segment ends on -x axis from above or starts down from -x axis
c        -2 if segment crosses -x axis from above
c Written by C. Mueller, USGS.
      KPSCR= 0
      if (Y1*Y2.gt.0.) return
      if (X1*Y2.eq.X2*Y1.and.X1*X2.le.0.) then
        KPSCR= 4
      elseif (Y1*Y2.lt.0.) then
        if (Y1.lt.0..and.X1*Y2.lt.X2*Y1) KPSCR= +2
        if (Y1.gt.0..and.X1*Y2.gt.X2*Y1) KPSCR= -2
      elseif (Y1.eq.0..and.X1.lt.0.) then
        if (Y2.lt.0.) KPSCR= -1
        if (Y2.gt.0.) KPSCR= +1
      elseif (Y2.eq.0..and.X2.lt.0.) then
        if (Y1.lt.0.) KPSCR= +1
        if (Y1.gt.0.) KPSCR= -1
      endif
      return
      end
c ===========================================================================

