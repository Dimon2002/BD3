SELECT t.n_post, t.kol, res.average_volume
FROM spj AS t
JOIN (
    -- Детали поставок для изделия из Рима
    SELECT spj.n_post, AVG(spj.kol) AS average_volume
    FROM spj
    JOIN j ON j.n_izd = spj.n_izd
    WHERE j.town = 'Рим'
    GROUP BY spj.n_post
) AS res ON res.n_post = t.n_post
WHERE t.kol >= 3 * res.average_volume
