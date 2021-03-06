CREATE TABLE IF NOT EXISTS `Users` (
  `id` INT NOT NULL AUTO_INCREMENT, 
  `type` INT NULL,
  `key` VARCHAR(45) NULL,
  `lastChange` BIGINT NULL,
  `lastAction` BIGINT NULL,
  `remouteAddress` VARCHAR(45) NULL,
  `userAgent` VARCHAR(128) NULL,
  `salt` VARCHAR(64) NULL,
  `login` VARCHAR(128) NULL,
  `pass` VARCHAR(128) NULL,
  PRIMARY KEY (`id`));
CREATE TABLE IF NOT EXISTS `Bans` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `userId` INT NULL,
  `roTime` BIGINT NULL,
  `closeAccess` TINYINT(1) NULL,
  `banCreationTime` BIGINT NULL,
  PRIMARY KEY (`id`),
  INDEX `fk_Bans_1_idx` (`userId` ASC),
  CONSTRAINT `fk_Bans_1`
    FOREIGN KEY (`userId`)
    REFERENCES `Users` (`id`)
    ON DELETE CASCADE
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_general_ci


