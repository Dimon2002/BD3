SELECT DISTINCT j.*
FROM spj
JOIN j ON j.n_izd = spj.n_izd
WHERE spj.n_det IN (
    SELECT p.n_det
    FROM p
    WHERE p.ves > 12
)
EXCEPT
SELECT DISTINCT j.*
FROM spj
JOIN j ON j.n_izd = spj.n_izd
WHERE NOT spj.n_det IN (
    SELECT p.n_det
    FROM p
    WHERE p.ves > 12
)