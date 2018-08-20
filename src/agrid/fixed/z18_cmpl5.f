      logical function z18_cmpl5(qln,qlt)
c ceus completeness zone #5; updated Apr2018
c Written by C. Mueller, USGS.
      parameter (nz=9)
      dimension zln(nz),zlt(nz)
      logical lxyin
      data ZLN(001),ZLT(001) /  -91.2094, 35.7449/ ! newmad
      data ZLN(002),ZLT(002) /  -90.2000, 34.8585/ ! newmad
      data ZLN(003),ZLT(003) /  -87.3, 35.0/
      data ZLN(004),ZLT(004) /  -81.6, 30.8/
      data ZLN(005),ZLT(005) / -100.0, 28.2/
      data ZLN(006),ZLT(006) / -100.0, 45.5/
      data ZLN(007),ZLT(007) /  -96.8, 44.8/
      data ZLN(008),ZLT(008) /  -91.5, 39.8/
      data ZLN(009),ZLT(009) /  -91.7, 36.5/
      z18_cmpl5= lxyin(qln,qlt,zln,zlt,nz)
      return
      end

