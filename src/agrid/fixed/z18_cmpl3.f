      logical function z18_cmpl3(qln,qlt)
c ceus completeness zone #3
c Written by C. Mueller, USGS.
      parameter (nz=6)
      dimension zln(nz),zlt(nz)
      logical lxyin
      data ZLN(001),ZLT(001) / -87.3, 35.0/
      data ZLN(002),ZLT(002) / -83.5, 40.4/
      data ZLN(003),ZLT(003) / -79.0, 40.4/
      data ZLN(004),ZLT(004) / -79.0, 37.2/
      data ZLN(005),ZLT(005) / -75.4, 35.8/
      data ZLN(006),ZLT(006) / -81.6, 30.8/
      z18_cmpl3= lxyin(qln,qlt,zln,zlt,nz)
      return
      end

