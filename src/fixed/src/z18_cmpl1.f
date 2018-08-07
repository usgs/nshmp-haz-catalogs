      logical function z18_cmpl1(qln,qlt)
c ceus completeness zone #1
c Written by C. Mueller, USGS.
      parameter (nz=8)
      dimension zln(nz),zlt(nz)
      logical lxyin
      data ZLN(001),ZLT(001) / -83.0, 47.0/
      data ZLN(002),ZLT(002) / -64.5, 50.9/
      data ZLN(003),ZLT(003) / -64.5, 45.0/
      data ZLN(004),ZLT(004) / -69.7, 43.4/
      data ZLN(005),ZLT(005) / -75.5, 42.2/
      data ZLN(006),ZLT(006) / -79.0, 40.4/
      data ZLN(007),ZLT(007) / -83.5, 40.4/
      data ZLN(008),ZLT(008) / -83.5, 42.4/
      z18_cmpl1= lxyin(qln,qlt,zln,zlt,nz)
      return
      end

