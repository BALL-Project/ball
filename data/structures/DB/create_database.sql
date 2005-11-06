-- MySQL dump 10.9
--
-- Host: diclofenac    Database: structures
-- ------------------------------------------------------
-- Server version	4.1.10a

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

--
-- Table structure for table `atom_coord`
--

DROP TABLE IF EXISTS `atom_coord`;
CREATE TABLE `atom_coord` (
  `id` int(11) NOT NULL auto_increment,
  `conformation_id` int(11) default NULL,
  `x` float default NULL,
  `y` float default NULL,
  `z` float default NULL,
  `atom_idx` int(2) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 MAX_ROWS=500000000;

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
  PRIMARY KEY  (`id`)
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
  PRIMARY KEY  (`id`)
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
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 MAX_ROWS=4294967295;

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

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;

