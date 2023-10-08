select count(distinct spj.n_det) kol_det 
from spj
where spj.n_post in (
    select DISTINCT spj.n_post 
    from spj
    join j ON j.n_izd = spj.n_izd
    where j.town = 'Париж'
)