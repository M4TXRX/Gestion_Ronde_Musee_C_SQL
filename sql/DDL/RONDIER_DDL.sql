-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema RONDIER
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `RONDIER` ;

-- -----------------------------------------------------
-- Schema RONDIER
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `RONDIER` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `RONDIER` ;

-- -----------------------------------------------------
-- Table `RONDIER`.`Pays`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Pays` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Pays` (
  `idPays` INT NOT NULL AUTO_INCREMENT,
  `Pays` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`idPays`),
  UNIQUE INDEX `idPays_UNIQUE` (`idPays` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Type_voie`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Type_voie` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Type_voie` (
  `idType_voie` INT NOT NULL AUTO_INCREMENT,
  `Type_voie` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`idType_voie`),
  UNIQUE INDEX `idType_voie_UNIQUE` (`idType_voie` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Adresse`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Adresse` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Adresse` (
  `idAdresse` INT NOT NULL AUTO_INCREMENT,
  `Numero` INT NOT NULL,
  `Adresse` VARCHAR(45) NOT NULL,
  `Complement_adresse` VARCHAR(45) NULL,
  `Code_postal` INT NOT NULL,
  `Ville` VARCHAR(45) NOT NULL,
  `Pays_idPays` INT NOT NULL,
  `Type_voie_idType_voie` INT NOT NULL,
  PRIMARY KEY (`idAdresse`),
  INDEX `fk_Adresse_Pays_idx` (`Pays_idPays` ASC),
  INDEX `fk_Adresse_Type_voie1_idx` (`Type_voie_idType_voie` ASC),
  CONSTRAINT `fk_Adresse_Pays`
    FOREIGN KEY (`Pays_idPays`)
    REFERENCES `RONDIER`.`Pays` (`idPays`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Adresse_Type_voie1`
    FOREIGN KEY (`Type_voie_idType_voie`)
    REFERENCES `RONDIER`.`Type_voie` (`idType_voie`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Agent`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Agent` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Agent` (
  `idAgent` INT NOT NULL AUTO_INCREMENT,
  `Nom` VARCHAR(45) NOT NULL,
  `Prenom` VARCHAR(45) NOT NULL,
  `Sexe` INT NOT NULL,
  `Salaire` INT NOT NULL,
  `Date_Naissance` DATETIME NOT NULL,
  `Date_Embauche` DATETIME NOT NULL,
  `Adresse_idAdresse` INT NOT NULL,
  PRIMARY KEY (`idAgent`),
  UNIQUE INDEX `idagent_UNIQUE` (`idAgent` ASC),
  INDEX `fk_agent_Adresse1_idx` (`Adresse_idAdresse` ASC),
  CONSTRAINT `fk_agent_Adresse1`
    FOREIGN KEY (`Adresse_idAdresse`)
    REFERENCES `RONDIER`.`Adresse` (`idAdresse`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Competence`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Competence` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Competence` (
  `idCompetence` INT NOT NULL AUTO_INCREMENT,
  `Nom` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`idCompetence`),
  UNIQUE INDEX `idCompetence_UNIQUE` (`idCompetence` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Telephone`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Telephone` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Telephone` (
  `idTelephone` INT NOT NULL AUTO_INCREMENT,
  `Numero_Telephone` VARCHAR(45) NOT NULL,
  `Agent_idAgent` INT NOT NULL,
  PRIMARY KEY (`idTelephone`),
  UNIQUE INDEX `idTelephone_UNIQUE` (`idTelephone` ASC),
  INDEX `fk_Telephone_Agent1_idx` (`Agent_idAgent` ASC),
  CONSTRAINT `fk_Telephone_Agent1`
    FOREIGN KEY (`Agent_idAgent`)
    REFERENCES `RONDIER`.`Agent` (`idAgent`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Ident_Agent`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Ident_Agent` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Ident_Agent` (
  `idIdent_Agent` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `UID_RFID` VARCHAR(45) NOT NULL,
  `Grade` VARCHAR(45) NOT NULL,
  `Date_Creation` DATE NOT NULL,
  `Agent_idAgent` INT NOT NULL,
  PRIMARY KEY (`idIdent_Agent`),
  INDEX `fk_Ident_Agent_Agent1_idx` (`Agent_idAgent` ASC),
  CONSTRAINT `fk_Ident_Agent_Agent1`
    FOREIGN KEY (`Agent_idAgent`)
    REFERENCES `RONDIER`.`Agent` (`idAgent`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Modele_Ronde`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Modele_Ronde` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Modele_Ronde` (
  `idModele_Ronde` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `Nom` VARCHAR(45) NOT NULL,
  `Duree` TIME NOT NULL,
  `ETAT` INT NOT NULL,
  `Ident_Agent_idIdent_Agent` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`idModele_Ronde`),
  INDEX `fk_Modele_Ronde_Ident_Agent1_idx` (`Ident_Agent_idIdent_Agent` ASC),
  CONSTRAINT `fk_Modele_Ronde_Ident_Agent1`
    FOREIGN KEY (`Ident_Agent_idIdent_Agent`)
    REFERENCES `RONDIER`.`Ident_Agent` (`idIdent_Agent`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Baton_Rondier`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Baton_Rondier` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Baton_Rondier` (
  `idBaton_Rondier` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `Adress_Mac` VARCHAR(45) NOT NULL,
  `ETAT` INT NULL,
  PRIMARY KEY (`idBaton_Rondier`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Instance_Ronde`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Instance_Ronde` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Instance_Ronde` (
  `idInstance_Ronde` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `Date_Creation` TIMESTAMP NOT NULL,
  `Etat` VARCHAR(45) NOT NULL,
  `Nom` VARCHAR(45) NOT NULL,
  `Duree` TIME NOT NULL,
  `Modele_Ronde_idModele_Ronde` INT UNSIGNED NOT NULL,
  `Baton_Rondier_idBaton_Rondier` INT UNSIGNED NOT NULL,
  `Ident_Agent_idIdent_Agent` INT UNSIGNED NOT NULL,
  PRIMARY KEY (`idInstance_Ronde`),
  INDEX `fk_Instance_Ronde_Modele_Ronde1_idx` (`Modele_Ronde_idModele_Ronde` ASC),
  INDEX `fk_Instance_Ronde_Baton_Rondier1_idx` (`Baton_Rondier_idBaton_Rondier` ASC),
  INDEX `fk_Instance_Ronde_Ident_Agent1_idx` (`Ident_Agent_idIdent_Agent` ASC),
  CONSTRAINT `fk_Instance_Ronde_Modele_Ronde1`
    FOREIGN KEY (`Modele_Ronde_idModele_Ronde`)
    REFERENCES `RONDIER`.`Modele_Ronde` (`idModele_Ronde`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Instance_Ronde_Baton_Rondier1`
    FOREIGN KEY (`Baton_Rondier_idBaton_Rondier`)
    REFERENCES `RONDIER`.`Baton_Rondier` (`idBaton_Rondier`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Instance_Ronde_Ident_Agent1`
    FOREIGN KEY (`Ident_Agent_idIdent_Agent`)
    REFERENCES `RONDIER`.`Ident_Agent` (`idIdent_Agent`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Point_Passage`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Point_Passage` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Point_Passage` (
  `idPoint_Passage` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `Ident_Tag` VARCHAR(45) NOT NULL,
  `Nom` VARCHAR(45) NOT NULL,
  `Cordo_X` INT NOT NULL,
  `Cordo_Y` INT NOT NULL,
  `Typ` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`idPoint_Passage`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Historique_Rondier`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Historique_Rondier` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Historique_Rondier` (
  `idPoint_Passage_has_Baton_Rondier` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `Point_Passage_idPoint_Passage` INT UNSIGNED NOT NULL,
  `Baton_Rondier_idBaton_Rondier` INT UNSIGNED NOT NULL,
  `Date_Creation` TIMESTAMP NOT NULL,
  `Date_event` DATETIME NOT NULL,
  INDEX `fk_Point_Passage_has_Baton_Rondier_Baton_Rondier1_idx` (`Baton_Rondier_idBaton_Rondier` ASC),
  INDEX `fk_Point_Passage_has_Baton_Rondier_Point_Passage1_idx` (`Point_Passage_idPoint_Passage` ASC),
  PRIMARY KEY (`idPoint_Passage_has_Baton_Rondier`),
  CONSTRAINT `fk_Point_Passage_has_Baton_Rondier_Point_Passage1`
    FOREIGN KEY (`Point_Passage_idPoint_Passage`)
    REFERENCES `RONDIER`.`Point_Passage` (`idPoint_Passage`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Point_Passage_has_Baton_Rondier_Baton_Rondier1`
    FOREIGN KEY (`Baton_Rondier_idBaton_Rondier`)
    REFERENCES `RONDIER`.`Baton_Rondier` (`idBaton_Rondier`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Modele_Ronde_has_Point_Passage`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Modele_Ronde_has_Point_Passage` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Modele_Ronde_has_Point_Passage` (
  `idModele_Ronde_has_Point_Passage` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `Date_creation` TIMESTAMP NOT NULL,
  `Num` INT NOT NULL,
  `Modele_Ronde_idModele_Ronde` INT UNSIGNED NOT NULL,
  `Point_Passage_idPoint_Passage` INT UNSIGNED NOT NULL,
  `Date_event` DATETIME NULL,
  PRIMARY KEY (`idModele_Ronde_has_Point_Passage`),
  INDEX `fk_Modele_Ronde_has_Point_Passage_Modele_Ronde1_idx` (`Modele_Ronde_idModele_Ronde` ASC),
  INDEX `fk_Modele_Ronde_has_Point_Passage_Point_Passage1_idx` (`Point_Passage_idPoint_Passage` ASC),
  CONSTRAINT `fk_Modele_Ronde_has_Point_Passage_Modele_Ronde1`
    FOREIGN KEY (`Modele_Ronde_idModele_Ronde`)
    REFERENCES `RONDIER`.`Modele_Ronde` (`idModele_Ronde`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Modele_Ronde_has_Point_Passage_Point_Passage1`
    FOREIGN KEY (`Point_Passage_idPoint_Passage`)
    REFERENCES `RONDIER`.`Point_Passage` (`idPoint_Passage`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Agent_has_Competence`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Agent_has_Competence` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Agent_has_Competence` (
  `IdAgent_has_Competence` INT NOT NULL AUTO_INCREMENT,
  `Agent_idAgent` INT NOT NULL,
  `Competence_idCompetence` INT NOT NULL,
  INDEX `fk_Agent_has_Competence1_Competence1_idx` (`Competence_idCompetence` ASC),
  INDEX `fk_Agent_has_Competence1_Agent1_idx` (`Agent_idAgent` ASC),
  PRIMARY KEY (`IdAgent_has_Competence`),
  CONSTRAINT `fk_Agent_has_Competence1_Agent1`
    FOREIGN KEY (`Agent_idAgent`)
    REFERENCES `RONDIER`.`Agent` (`idAgent`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Agent_has_Competence1_Competence1`
    FOREIGN KEY (`Competence_idCompetence`)
    REFERENCES `RONDIER`.`Competence` (`idCompetence`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `RONDIER`.`Instance_Ronde_has_Point_Passage`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `RONDIER`.`Instance_Ronde_has_Point_Passage` ;

CREATE TABLE IF NOT EXISTS `RONDIER`.`Instance_Ronde_has_Point_Passage` (
  `idInstance_Ronde_has_Point_Passage` INT NOT NULL AUTO_INCREMENT,
  `Point_Passage_idPoint_Passage` INT UNSIGNED NOT NULL,
  `Instance_Ronde_idInstance_Ronde` INT UNSIGNED NOT NULL,
  `Date_creation` TIMESTAMP NOT NULL,
  `Date_event` DATETIME NOT NULL,
  INDEX `fk_Point_Passage_has_Instance_Ronde1_Instance_Ronde1_idx` (`Instance_Ronde_idInstance_Ronde` ASC),
  INDEX `fk_Point_Passage_has_Instance_Ronde1_Point_Passage1_idx` (`Point_Passage_idPoint_Passage` ASC),
  PRIMARY KEY (`idInstance_Ronde_has_Point_Passage`),
  CONSTRAINT `fk_Point_Passage_has_Instance_Ronde1_Point_Passage1`
    FOREIGN KEY (`Point_Passage_idPoint_Passage`)
    REFERENCES `RONDIER`.`Point_Passage` (`idPoint_Passage`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Point_Passage_has_Instance_Ronde1_Instance_Ronde1`
    FOREIGN KEY (`Instance_Ronde_idInstance_Ronde`)
    REFERENCES `RONDIER`.`Instance_Ronde` (`idInstance_Ronde`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
