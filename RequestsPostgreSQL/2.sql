UPDATE p set cvet = (
    CASE WHEN p.town = 'Лондон'
        THEN (-- Цвет из рима
            SELECT rp.cvet
            FROM p AS rp
            WHERE rp.town = 'Рим'
            ORDER BY rp.cvet 
            LIMIT 1
        )
        ELSE (-- Цвет из Лондона
            SELECT lp.cvet
            FROM p AS lp
            WHERE lp.town = 'Лондон'
            ORDER BY lp.cvet 
            LIMIT 1
        )
    END
)    
FROM p
WHERE p.town = 'Рим'
    OR p.town = 'Лондон' 