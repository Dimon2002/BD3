SELECT j.n_izd
FROM j
EXCEPT
SELECT DISTINCT spj.n_izd
FROM spj
WHERE spj.n_post IN (
    -- Поставщики, поставлявшие красные изделия
    SELECT DISTINCT a.n_post
    FROM spj AS a
    JOIN p ON p.n_det = a.n_det
    WHERE p.cvet = 'Красный'
)