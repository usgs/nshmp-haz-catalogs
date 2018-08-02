clear all
close all

lon_arr=240:0.1:250;
lat_arr=40:.1:50;
nev=1e5;
%pi
cax=[8.9 10.6];

% init
cnt=1;
for ii=1:length(lat_arr)
  for jj=1:length(lon_arr)
    lon_all(cnt)=lon_arr(jj);
    lat_all(cnt)=lat_arr(ii);
    cnt=cnt+1;
  end
end
npts=cnt-1;

% 
deg2rad=pi/180;
sum_area=0;
for ii=1:npts
  sum_area=sum_area+cos(lat_all(ii)*deg2rad);
end

% distribute single event over this area
avg_lat=(min(lat_all)+max(lat_all))/2;
for ii=1:npts
  rate_uniform(ii)=nev/npts;
  scale_fac(ii)=cos(lat_all(ii)*deg2rad)/cos(avg_lat*deg2rad);
  rate_scaled(ii)=rate_uniform(ii)*scale_fac(ii);
  diff_rate(ii)=rate_scaled(ii)-rate_uniform(ii);
end

tot_scaled=sprintf('Sum latitude-scaled rates: %.2f\n', sum(rate_scaled));
tot_uniform=sprintf('Sum latitude-uniform rates: %.2f\n', sum(rate_uniform));
disp(tot_uniform)
disp(tot_scaled)

figure
sval=15;
% uniform
subplot(2,2,1),
scatter(lon_all,lat_all,sval,rate_uniform),
colorbar
caxis(cax),
title('uniform')

% scaled
subplot(2,2,2),
scatter(lon_all,lat_all,sval,rate_scaled),
colorbar
caxis(cax),
title('scaled by lat')

% difference
subplot(2,2,3),
scatter(lon_all,lat_all,sval,diff_rate),
colorbar
title('diff')

% scaling
subplot(2,2,4),
scatter(lon_all,lat_all,sval,scale_fac),
colorbar
title('scaling factor')

