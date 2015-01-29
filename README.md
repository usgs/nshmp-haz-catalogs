## National Seismic Hazard Model (NSHM) Earthquake Catalogs

With each release of the NSHM, a variety of earthquake catalogs are compiled and subsequently processed to remove duplicate, dependent (foreshocks and aftershocks), and anthropogenic events. Gridded seismicity source models in hazard calculations are derived from final, filtered catalogs. The tables below outline the processing steps applied to each catalog available in this repository.

### [2014 NSHM Catalogs](https://github.com/usgs/nshmp-haz-catalogs/tree/master/2014)

Details of the earthquake catalogs used in constructing the 2014 NSHM, by file. For 2014, anthropogenic events are classified as mining-related and/or explosions (M&E), or potentially fluid induced (PFI). Declustered catalogs (dependent events removed) only include M ≥ 2.5 events.

Name | Region | Duplicate | Dependent | Anthro. M&E | Anthro. PFI
:----|:------:|:---------:|:---------:|:-----------:|:----------:
[2014_emm.c2](https://raw.githubusercontent.com/usgs/nshmp-haz-catalogs/master/2014/2014_emm.c2.txt)|CEUS|❌|✅|❌|✅
[2014_emm.c3](https://raw.githubusercontent.com/usgs/nshmp-haz-catalogs/master/2014/2014_emm.c3.txt)|CEUS|❌|❌|❌|✅
[2014_emm.c4](https://raw.githubusercontent.com/usgs/nshmp-haz-catalogs/master/2014/2014_emm.c4.txt)|CEUS|❌|❌|❌|❌
[2014_wmm.c2](https://raw.githubusercontent.com/usgs/nshmp-haz-catalogs/master/2014/2014_wmm.c2.txt)|WUS |❌|✅|❌|✅
[2014_wmm.c3](https://raw.githubusercontent.com/usgs/nshmp-haz-catalogs/master/2014/2014_wmm.c3.txt)|WUS |❌|❌|❌|✅
[2014_wmm.c4](https://raw.githubusercontent.com/usgs/nshmp-haz-catalogs/master/2014/2014_wmm.c4.txt)|WUS |❌|❌|❌|❌

✅ = events included  
❌ = events deleted  
WUS = Western US  
CEUS = Central & Eastern US  

### [2008 NSHM Catalogs](https://github.com/usgs/nshmp-haz-catalogs/tree/master/2008)

Details of the earthquake catalogs used in constructing the 2008 NSHM, by file.

Name | Region | Duplicates | Aftershocks | Anthropogenic | Notes
:----|:------:|:----------:|:-----------:|:-------------:|:----:
[emb.c_nd](https://github.com/usgs/nshmp-haz-catalogs/raw/master/2008/2008_emb.c_nd.txt)|CEUS|❌|✅|✅|1
[emb.cc](https://github.com/usgs/nshmp-haz-catalogs/raw/master/2008/2008_emb.cc.txt)  |CEUS|❌|❌|❌|2
[wmm.c_nd](https://github.com/usgs/nshmp-haz-catalogs/raw/master/2008/2008_wmm.c_nd.txt)|WUS |❌|✅|✅|1
[wmm.cc](https://github.com/usgs/nshmp-haz-catalogs/raw/master/2008/2008_wmm.cc.txt)  |WUS |❌|❌|❌|3

✅ = events included  
❌ = events deleted  
WUS = Western US  
CEUS = Central & Eastern US  

1 = Explosions and mining-related events included  
2 = mbLg ≥ 3  
3 = Mw ≥ 4  
