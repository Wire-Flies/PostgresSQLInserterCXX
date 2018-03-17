-- For getting all different airports
select * from  ((select distinct schd_from as airport from flights) union (select distinct schd_to as airport from flights union (select distinct real_to from flights))) t0;



