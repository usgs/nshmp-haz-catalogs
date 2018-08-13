      logical function inetennz(qln,qlt)
c Is point (qln,qlt) inside the eastern Tennessee zone?
c Boundary from M. Chapman (~6/95).
c Written by C. Mueller, USGS.
      parameter (nz=4)
      dimension zln(nz),zlt(nz)
      logical lxyin
      data ZLN(001),ZLT(001) / -85.90, 34.45/
      data ZLN(002),ZLT(002) / -83.45, 36.60/
      data ZLN(003),ZLT(003) / -83.00, 36.35/
      data ZLN(004),ZLT(004) / -85.30, 34.10/
      inetennz= lxyin(qln,qlt,zln,zlt,nz)
      return
      end

