CREATE DATABASE ART_PROJ_PART_2;
#CREATE DATABASE ART_PROJ_PART_2;
USE ART_PROJ_PART_2;

CREATE TABLE artist(
	name	VARCHAR(16)		NOT NULL,
    address	VARCHAR(24)		NOT NULL,
	phone   VARCHAR(12)		NOT NULL,
	age     INT	       		NOT NULL,
	birth_place VARCHAR(30)		NOT NULL,
	art_style   VARCHAR(15)		NOT NULL
);
CREATE TABLE art_work(
	artist	VARCHAR(16)		NOT NULL,
    title	VARCHAR(16)		NOT NULL,
	art_type   VARCHAR(15)		NOT NULL,
	date     VARCHAR(11)	       		NOT NULL,
	acquired_date VARCHAR(11)		NOT NULL,
	price   VARCHAR(15)		NOT NULL,
    location   VARCHAR(15)		NOT NULL
);
CREATE TABLE art_show(
	artist	VARCHAR(16)		NOT NULL,
	date     VARCHAR(11)	       		NOT NULL,
	time VARCHAR(10)		NOT NULL,
    location   VARCHAR(15)		NOT NULL,
	contact   VARCHAR(30)		NOT NULL,
	contact_phone   VARCHAR(21)		NOT NULL
);
CREATE TABLE customer(
	number	INT		NOT NULL,
	phone     VARCHAR(12)	 NOT NULL,
    preferences   VARCHAR(30)		NOT NULL
);

SHOW TABLES;

INSERT INTO customer VALUES(01,"714-354-9753","painting");
INSERT INTO customer VALUES(02,"951-974-5498","sculpture");
INSERT INTO customer VALUES(03,"715-386-4739","drawing");
INSERT INTO customer VALUES(04,"714-482-9613","painting");

INSERT INTO artist VALUES("Pablo Jeffes","3423 Loren Dr", "472-383-2743",24,"Medocino, CA", "painting");
INSERT INTO artist VALUES("John Redd","9472 Autzen Rd", "951-332-8374",30,"Portland, OR", "sculpture");
INSERT INTO artist VALUES("Lejourne Johnson","28421 Kennedy Way", "504-974-3821",27,"Dallas, TX", "drawing");
INSERT INTO artist VALUES("Kyle Lee","39130 Culver Ave", "949-393-8473",27,"Irvine, CA", "painting");

INSERT INTO art_work VALUES("Pablo Jeffes", "fall years", "painting", '12/04/2019', '12/10/2019',"20,000","SanFran");
INSERT INTO art_work VALUES("John Redd", "Mr Pob", "sculpture", '03/13/2019', '03/27/2019',"5,000","Portland");
INSERT INTO art_work VALUES("Lejourne Johnson", "Miss Pretty", "drawing", '02/1/2019', '02/10/2019',"15,000","Austin");
INSERT INTO art_work VALUES("Kyle Lee", "Suburb Vibez", "painting", '05/20/2019', '06/01/2019',"16,000","Malibu");

INSERT INTO art_show VALUES("Pablo Jeffes", '12/05/2019',"1:30pm", "San Francisco", "Marcos Martinez", "213-817-3835");
INSERT INTO art_show VALUES("John Redd", '03/14/2019', "3:00pm", "Portland","Henry Longers", "320-5853-8372");
INSERT INTO art_show VALUES("Lejourne Johnson", '02/5/2019',"6:00pm","Austin", "Derrick Johnson", "596-2383-9453");
INSERT INTO art_show VALUES("Kyle Lee", '05/22/2019', "5:00pm","Malibu","Dwight Scott", "169-327-8472");


#DELETE FROM art_show WHERE artist='John Redd';

#DROP TABLE art_work;
#DROP TABLE art_show;
#SELECT * from artist;
#SELECT * from art_show;
#SELECT * from art_work;
#SELECT * from customer;


