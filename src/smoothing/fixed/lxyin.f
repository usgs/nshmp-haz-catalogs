      logical function lxyin(x,y,px,py,n)
c Is point (x,y) inside the (unclosed) polygon (px,py)?
c See "Application of the winding-number algorithm...",
c  by Godkin and Pulli, BSSA, pp. 1845-1848, 1984.
c lxyin= .true. if (x,y) is inside or on polygon, .false. otherwise.
c Written by C. Mueller, USGS.
      dimension px(n),py(n)
      lxyin= .true.
      ksum= 0
      do 1 i=1,n-1
        k= kpscr(px(i)-x,py(i)-y,px(i+1)-x,py(i+1)-y)
        if (k.eq.4) return
        ksum= ksum+k
1       continue
      k= kpscr(px(n)-x,py(n)-y,px(1)-x,py(1)-y)
      if (k.eq.4) return
      ksum= ksum+k
      if (ksum.eq.0) lxyin= .false.
      return
      end
      integer function kpscr(x1,y1,x2,y2)
c Compute the signed crossing number of the segment from (x1,y1) to (x2,y2).
c See "Application of the winding-number algorithm...",
c  by Godkin and Pulli, BSSA, pp. 1845-1848, 1984.
c kpscr= +4 if segment passes through the origin
c        +2 if segment crosses -x axis from below
c        +1 if segment ends on -x axis from below or starts up from -x axis
c         0 if no crossing
c        -1 if segment ends on -x axis from above or starts down from -x axis
c        -2 if segment crosses -x axis from above
      kpscr= 0
      if (y1*y2.gt.0.) return
      if (x1*y2.eq.x2*y1.and.x1*x2.le.0.) then
        kpscr= 4
      elseif (y1*y2.lt.0.) then
        if (y1.lt.0..and.x1*y2.lt.x2*y1) kpscr= +2
        if (y1.gt.0..and.x1*y2.gt.x2*y1) kpscr= -2
      elseif (y1.eq.0..and.x1.lt.0.) then
        if (y2.lt.0.) kpscr= -1
        if (y2.gt.0.) kpscr= +1
      elseif (y2.eq.0..and.x2.lt.0.) then
        if (y1.lt.0.) kpscr= +1
        if (y1.gt.0.) kpscr= -1
      endif
      return
      end
