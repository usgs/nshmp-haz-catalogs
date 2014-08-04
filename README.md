## National Seismic Hazard Model (NSHM) Earthquake Catalogs

With each release of the NSHM, a variety of earthquake catalogs are compiled and subsequently processed to remove duplicate, dependent (foreshocks and aftershocks), and anthropogenic events. Gridded seismicity source models in hazard calculations are derived from final, filtered catalogs. The tables below outline the processing steps applied to each catalog available in this repository.

### [2014 NSHM Catalogs](https://github.com/usgs/nshmp-haz-catalogs/tree/master/2014)

Details of the earthquake catalogs used in constructing the 2014 NSHM, by file. For 2014, anthropogenic events are classified as mining-related and/or explosions (M&E), or potentially fluid induced (PFI). Declustered catalogs (dependent events removed) only include M ≥ 2.5 events.

Name | Region | Duplicate | Dependent | Anthro. M&E | Anthro. PFI
:----|:------:|:---------:|:---------:|:-----------:|:----------:
2014_emm.c2|CEUS|❌|✅|❌|✅
2014_emm.c3|CEUS|❌|❌|❌|✅
2014_emm.c4|CEUS|❌|❌|❌|❌
2014_wmm.c2|WUS |❌|✅|❌|✅
2014_wmm.c3|WUS |❌|❌|❌|✅
2014_wmm.c4|WUS |❌|❌|❌|❌

✅ = events included  
❌ = events deleted  
WUS = Western US  
CEUS = Central & Eastern US  

### [2008 NSHM Catalogs](https://github.com/usgs/nshmp-haz-catalogs/tree/master/2008)

Details of the earthquake catalogs used in constructing the 2008 NSHM, by file.

Name | Region | Duplicates | Aftershocks | Anthropogenic | Notes
:----|:------:|:----------:|:-----------:|:-------------:|:----:
emb.c_nd|CEUS|❌|✅|✅|1
emb.cc  |CEUS|❌|❌|❌|2
wmm.c_nd|WUS |❌|✅|✅|1
wmm.cc  |WUS |❌|❌|❌|3

✅ = events included  
❌ = events deleted  
WUS = Western US  
CEUS = Central & Eastern US  

1 = Explosions and mining-related events included  
2 = mbLg ≥ 3  
3 = Mw ≥ 4  
