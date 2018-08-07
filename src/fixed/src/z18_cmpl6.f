      logical function z18_cmpl6(qln,qlt)
c ceus completeness zone #6; updated Apr2018
c Written by C. Mueller, USGS.
      parameter (nz=47)
      dimension zln(nz),zlt(nz)
      logical lxyin
      data ZLN(001),ZLT(001) / -110.00, 45.50/ ! in_ce2
      data ZLN(002),ZLT(002) / -110.10, 44.90/ ! in_ce2
      data ZLN(003),ZLT(003) / -109.90, 44.55/ ! in_ce2
      data ZLN(004),ZLT(004) / -109.85, 44.15/ ! in_ce2
      data ZLN(005),ZLT(005) / -110.28, 43.17/ ! in_ce2
      data ZLN(006),ZLT(006) / -110.72, 42.18/ ! in_ce2
      data ZLN(007),ZLT(007) / -110.70, 39.55/ ! in_ce2
      data ZLN(008),ZLT(008) / -111.00, 39.40/ ! in_ce2
      data ZLN(009),ZLT(009) / -111.50, 38.50/ ! in_ce2
      data ZLN(010),ZLT(010) / -111.60, 37.95/ ! in_ce2
      data ZLN(011),ZLT(011) / -111.40, 37.45/ ! in_ce2
      data ZLN(012),ZLT(012) / -110.90, 37.05/ ! in_ce2
      data ZLN(013),ZLT(013) / -110.90, 36.80/ ! in_ce2
      data ZLN(014),ZLT(014) / -111.50, 36.20/ ! in_ce2
      data ZLN(015),ZLT(015) / -111.35, 35.70/ ! in_ce2
      data ZLN(016),ZLT(016) / -111.00, 35.25/ ! in_ce2
      data ZLN(017),ZLT(017) / -110.25, 34.80/ ! in_ce2
      data ZLN(018),ZLT(018) / -109.70, 34.55/ ! in_ce2
      data ZLN(019),ZLT(019) / -109.30, 34.55/ ! in_ce2
      data ZLN(020),ZLT(020) / -109.00, 34.70/ ! in_ce2
      data ZLN(021),ZLT(021) / -108.45, 35.00/ ! in_ce2
      data ZLN(022),ZLT(022) / -107.60, 35.50/ ! in_ce2
      data ZLN(023),ZLT(023) / -107.15, 35.90/ ! in_ce2
      data ZLN(024),ZLT(024) / -106.90, 36.25/ ! in_ce2
      data ZLN(025),ZLT(025) / -106.85, 36.65/ ! in_ce2
      data ZLN(026),ZLT(026) / -107.00, 37.00/ ! in_ce2
      data ZLN(027),ZLT(027) / -107.55, 37.30/ ! in_ce2
      data ZLN(028),ZLT(028) / -107.75, 37.55/ ! in_ce2
      data ZLN(029),ZLT(029) / -107.80, 37.85/ ! in_ce2
      data ZLN(030),ZLT(030) / -107.50, 38.40/ ! in_ce2
      data ZLN(031),ZLT(031) / -106.25, 38.55/ ! in_ce2
      data ZLN(032),ZLT(032) / -105.90, 38.30/ ! in_ce2
      data ZLN(033),ZLT(033) / -105.62, 38.08/ ! in_ce2
      data ZLN(034),ZLT(034) / -105.28, 37.68/ ! in_ce2
      data ZLN(035),ZLT(035) / -105.09, 37.29/ ! in_ce2
      data ZLN(036),ZLT(036) / -105.12, 36.81/ ! in_ce2
      data ZLN(037),ZLT(037) / -105.17, 36.59/ ! in_ce2
      data ZLN(038),ZLT(038) / -105.22, 35.95/ ! in_ce2
      data ZLN(039),ZLT(039) / -105.20, 35.00/ ! in_ce2
      data ZLN(040),ZLT(040) / -105.55, 34.35/ ! in_ce2
      data ZLN(041),ZLT(041) / -105.55, 33.85/ ! in_ce2
      data ZLN(042),ZLT(042) / -105.35, 33.25/ ! in_ce2
      data ZLN(043),ZLT(043) / -104.50, 31.70/ ! in_ce2
      data ZLN(044),ZLT(044) / -102.10, 29.60/ ! in_ce2
      data ZLN(045),ZLT(045) / -101.00, 29.20/ ! in_ce2
      data ZLN(046),ZLT(046) / -100.0, 28.2/
      data ZLN(047),ZLT(047) / -100.0, 45.5/
      z18_cmpl6= lxyin(qln,qlt,zln,zlt,nz)
      return
      end

