      logical function z18_cmpl4(qln,qlt)
c ceus completeness zone #4
c Written by C. Mueller, USGS.
      parameter (nz=8)
      dimension zln(nz),zlt(nz)
      logical lxyin
      data ZLN(001),ZLT(001) / -90.2000, 34.8585/ ! newmad
      data ZLN(002),ZLT(002) / -91.2094, 35.7449/ ! newmad
      data ZLN(003),ZLT(003) / -91.7, 36.5/
      data ZLN(004),ZLT(004) / -91.5, 39.8/
      data ZLN(005),ZLT(005) / -90.2, 42.4/
      data ZLN(006),ZLT(006) / -83.5, 42.4/
      data ZLN(007),ZLT(007) / -83.5, 40.4/
      data ZLN(008),ZLT(008) / -87.3, 35.0/
      z18_cmpl4= lxyin(qln,qlt,zln,zlt,nz)
      return
      end

