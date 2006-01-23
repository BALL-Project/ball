
/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

CREATE DATABASE IF NOT EXISTS BALL_DBINTERFACE_TEST;
USE BALL_DBINTERFACE_TEST;

--
-- Table structure for table `charge_generation`
--

DROP TABLE IF EXISTS `charge_generation`;
CREATE TABLE `charge_generation` (
  `method_id` int(11) NOT NULL auto_increment,
  `method` text,
  `parameters` text,
  PRIMARY KEY  (`method_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Table structure for table `charges`
--

DROP TABLE IF EXISTS `charges`;
CREATE TABLE `charges` (
  `id` int(11) NOT NULL auto_increment,
  `topology_id` int(11) default NULL,
  `method_id` int(11) default NULL,
  `charges` blob,
  PRIMARY KEY  (`id`),
  KEY `topology_id` (`topology_id`),
  KEY `method_id` (`method_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Table structure for table `conformation_generation`
--

DROP TABLE IF EXISTS `conformation_generation`;
CREATE TABLE `conformation_generation` (
  `method_id` int(11) NOT NULL auto_increment,
  `method` text,
  `parameters` text,
  PRIMARY KEY  (`method_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Table structure for table `conformations`
--

DROP TABLE IF EXISTS `conformations`;
CREATE TABLE `conformations` (
  `id` int(11) NOT NULL auto_increment,
  `topology_id` int(11) default NULL,
  `method_id` int(11) default NULL,
  `energy` float default NULL,
  `coordinates` blob,
  PRIMARY KEY  (`id`),
  KEY `topology_id` (`topology_id`),
  KEY `method_id` (`method_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 MAX_ROWS=100000000;

--
-- Table structure for table `connection_table_atoms`
--

DROP TABLE IF EXISTS `connection_table_atoms`;
CREATE TABLE `connection_table_atoms` (
  `id` int(11) NOT NULL auto_increment,
  `topology_id` int(11) default NULL,
  `atom_number` int(2) default NULL,
  `element` char(2) default NULL,
  `formal_charge` int(1) default NULL,
  `name` char(8) default NULL,
  PRIMARY KEY  (`id`),
  KEY `topology_id` (`topology_id`),
  KEY `atom_number` (`atom_number`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 MAX_ROWS=4294967295;

--
-- Table structure for table `connection_table_bonds`
--

DROP TABLE IF EXISTS `connection_table_bonds`;
CREATE TABLE `connection_table_bonds` (
  `id` int(11) NOT NULL auto_increment,
  `topology_id` int(11) default NULL,
  `bond_number` int(2) default NULL,
  `atom1` int(2) default NULL,
  `atom2` int(2) default NULL,
  `bond_order` int(1) default NULL,
  `stereo` int(1) default NULL,
  PRIMARY KEY  (`id`),
  KEY `topology_id` (`topology_id`),
  KEY `bond_number` (`bond_number`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 MAX_ROWS=4294967295;

--
-- Table structure for table `fingerprints`
--

DROP TABLE IF EXISTS `fingerprints`;
CREATE TABLE `fingerprints` (
  `ID` varchar(255) NOT NULL default '',
  `torsion_key` longblob NOT NULL,
  `bit_maccs` blob,
  `uck` varchar(32) default NULL,
  `formula` varchar(40) default NULL,
  `struc` longblob,
  `weight` float default NULL,
  `torsion_count` longblob,
  `structure_id` int(11) NOT NULL default '0',
  `torsion_bind` longblob,
  `frowns` longblob,
  `name` varchar(100) default NULL,
  `filter` text,
  `smiles` blob,
  PRIMARY KEY  (`structure_id`),
  KEY `id_index` (`ID`),
  KEY `comp_index` (`ID`,`formula`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 MAX_ROWS=100000000 AVG_ROW_LENGTH=2000;

--
-- Table structure for table `singular`
--

DROP TABLE IF EXISTS `singular`;
CREATE TABLE `singular` (
  `ID` varchar(255) NOT NULL default '',
  `torsion_key` longblob NOT NULL,
  `bit_maccs` blob,
  `uck` varchar(32) default NULL,
  `formula` varchar(40) default NULL,
  `struc` longblob,
  `weight` float default NULL,
  `torsion_count` longblob,
  `count` int(11) NOT NULL default '0',
  UNIQUE KEY `uck_index` (`uck`),
  KEY `ID_index` (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Table structure for table `topology`
--

DROP TABLE IF EXISTS `topology`;
CREATE TABLE `topology` (
  `topology_id` int(11) NOT NULL auto_increment,
  `source_id` text,
  `name` text,
  PRIMARY KEY  (`topology_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Create default entry for default conformation generation method
-- and default charge creation method.
--
INSERT INTO `conformation_generation` (method_id, method, parameters) VALUES (0, "default", "default");
INSERT INTO `charge_generation` (method_id, method, parameters) VALUES (0, "default", "default");

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

