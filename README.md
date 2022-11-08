# Temperature-control 🗽

This is mini app to control temperature in home. As a user, you can set temperature in your room and controller will do everything for you. The controller get data of system central heating and controls temperature flowing water by means of water pumps. Using Web Server You're going to get new notifications about working condition elements in central heating.

# Technologies ⚙️

+ C
+ ESP IDF 
+ WEB SERVER
+ ADC  
+ HTML
+ CSS
+ JS

# Screenshots 📺

![Panel](https://user-images.githubusercontent.com/90866275/200567230-bc04f264-0fb2-40f6-b886-385137128600.png)

![Panel_error](https://user-images.githubusercontent.com/90866275/200567328-1a3c43cf-0a0c-453b-9ae1-a2c2e7d00a8f.png)

![Logowanie](https://user-images.githubusercontent.com/90866275/200567351-3e2ea032-3be3-44e3-8c79-c32a405f9599.png)

![Panel_temperature](https://user-images.githubusercontent.com/90866275/200567405-b1e57f72-50ac-4218-83b2-f7f9607bd2e6.png)

# Installation 💿 

```bash

git clone https://github.com/bartosz1998/temperature-control.git

idf.py build

idf.py -p PORT flash

idf.py monitor
```
