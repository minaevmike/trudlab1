SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema mydb
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `mydb` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `mydb` ;

-- -----------------------------------------------------
-- Table `mydb`.`managers`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`managers` (
  `id` INT NOT NULL,
  `name` VARCHAR(80) NOT NULL,
  `password` VARCHAR(80) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb`.`zayvki`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`zayvki` (
  `id` INT NOT NULL,
  `number` VARCHAR(45) NOT NULL,
  `text` TEXT NOT NULL,
  `managers_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_zayvki_managers_idx` (`managers_id` ASC),
  CONSTRAINT `fk_zayvki_managers`
    FOREIGN KEY (`managers_id`)
    REFERENCES `mydb`.`managers` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

USE `mydb` ;

-- -----------------------------------------------------
-- Placeholder table for view `mydb`.`view1`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `mydb`.`view1` (`id` INT);

-- -----------------------------------------------------
-- View `mydb`.`view1`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb`.`view1`;
USE `mydb`;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
