INSERT INTO `Adresse` (`idAdresse`,`Numero`, `Adresse`, `Complement_adresse`, `Code_postal`, `Ville`,`Pays_idPays`, `Type_voie_idType_voie`) 
VALUES (1,'22', 'Des pingouins', NULL, '75012', 'Paris', (SELECT `idPays` from `Pays` WHERE `Pays`='France'), (SELECT `idType_voie` from `Type_voie` WHERE `Type_voie`='rue')),
(2, '44', 'Des clowns', '1er étage', '44000', 'Nantes',(SELECT `idPays` from `Pays` WHERE `Pays`='France'), (SELECT `idType_voie` from `Type_voie` WHERE `Type_voie`='avenue'))
,(3, '33', 'Des artistes', NULL, '28000', 'Chartres', (SELECT `idPays` from `Pays` WHERE `Pays`='France'),(SELECT `idType_voie` from `Type_voie` WHERE `Type_voie`='lieu-dit')),
(4, '1', 'Lemercier', NULL, '75013', 'Paris',(SELECT `idPays` from `Pays` WHERE `Pays`='France'),(SELECT `idType_voie` from `Type_voie` WHERE `Type_voie`='rue'));