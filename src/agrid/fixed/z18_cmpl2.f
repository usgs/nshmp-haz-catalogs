      logical function z18_cmpl2(qln,qlt)
c ceus completeness zone #2
c Written by C. Mueller, USGS.
      parameter (nz=7)
      dimension zln(nz),zlt(nz)
      logical lxyin
      data ZLN(001),ZLT(001) / -75.4, 35.8/
      data ZLN(002),ZLT(002) / -79.0, 37.2/
      data ZLN(003),ZLT(003) / -79.0, 40.4/
      data ZLN(004),ZLT(004) / -75.5, 42.2/
      data ZLN(005),ZLT(005) / -69.7, 43.4/
      data ZLN(006),ZLT(006) / -69.6, 41.2/
      data ZLN(007),ZLT(007) / -73.6, 40.2/
      z18_cmpl2= lxyin(qln,qlt,zln,zlt,nz)
      return
      end

