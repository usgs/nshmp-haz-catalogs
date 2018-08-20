      logical function innewmad(qln,qlt)
c Is point (qln,qlt) inside the New Madrid zone?
c Original craton-margin boundary from A. Frankel.
c This is a tectonically (rather than seismically) defined zone.
c  Consider pushing the northmost point up to the intersection
c  with the Grayville & Rough Creek grabens (see inmarggc.f),
c  renaming this the "Reelfoot Rift" zone, and then defining
c  another zone for New Madrid seismicity?
c Written by C. Mueller, USGS.
      parameter (nz=13)
      dimension zln(nz),zlt(nz)
      logical lxyin
      data ZLN(001),ZLT(001) / -91.2094, 35.7449/
      data ZLN(002),ZLT(002) / -89.3703, 37.1120/
      data ZLN(003),ZLT(003) / -89.3074, 37.2101/
      data ZLN(004),ZLT(004) / -89.1686, 37.3347/
      data ZLN(005),ZLT(005) / -88.9518, 37.4646/
      data ZLN(006),ZLT(006) / -88.6800, 37.5742/
      data ZLN(007),ZLT(007) / -88.5285, 37.6229/
      data ZLN(008),ZLT(008) / -88.3550, 37.4996/
      data ZLN(009),ZLT(009) / -88.0432, 37.1599/
      data ZLN(010),ZLT(010) / -87.9460, 36.9439/
      data ZLN(011),ZLT(011) / -87.9166, 36.6190/
      data ZLN(012),ZLT(012) / -87.9856, 36.4745/
      data ZLN(013),ZLT(013) / -90.20, 34.8585/
      innewmad= lxyin(qln,qlt,zln,zlt,nz)
      return
      end

