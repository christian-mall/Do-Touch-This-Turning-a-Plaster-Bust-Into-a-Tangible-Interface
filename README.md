# Do Touch This: Turning a Plaster Bust Into a Tangible Interface

This readme contains a summary of the prototyping work that was done for the paper to create the prototype of the plaster bust. Alongside, all of the findings of the authors regarding aspects that one has to pay attention to as well as some dos and don'ts will be shared. The final code can be found under "codes/interactive_bust.ino". One general note: The program "Fritzing" (https://fritzing.org) was used to create the wiring diagrams and some parts, e.g., the copper pads are custom-made by the authors because they didn't exist pre-made in the program.

## Table of Contents

#### 1. Silicone Mold From the Bust
#### 2. Capacitive Sensing
#### 3. Output Modalities
#### 4. Creation of the Prototype of the Interactive Bust
#### 5. Limitations

## 1. Silicone Mold From the Bust

In this section, it will be described how a silicone mold was created from the original bust from the museum. This mold was used to make the plaster castings.

The first step was to cut out a base plate (in this case made out of wood) where the head can rest on. To fixate the head onto the board and to prevent any silicone from leaking under the head before it fully cures, Plasticine (a sort of modeling clay) was used. In order for it to be malleable, it was put in the microwave for a short period of time which made working with it way easier. Then the clay was used to seal all of the crevices between the head and the base plate (see figure below).

<img src="https://user-images.githubusercontent.com/44895720/90312368-3b8a3d00-df04-11ea-8528-eccf78b39910.jpeg" width="500">

The next step was to build a wooden case around the bust to build a vessel for the silicone. This can be seen in the figure below.

<img src="https://user-images.githubusercontent.com/44895720/90312425-a5a2e200-df04-11ea-916f-551afaf232df.jpeg" height="450">

The silicone that was used is an "SF33 - RTV2" silicone. This type of silicone is milky-transparent, very durable, and heat-resistant. It’s a silicone that is made for making silicone molds and it consists of two parts:

* a base
* a catalyst

Once these two parts are properly mixed in a 1:1 ratio the silicone cures in around 3 hours at room temperature. Higher temperatures speed up the curing process. Normal silicone needs air to cure which would have been problematic here because the final mold is quite thick. The "SF33" means that its Shore hardness is "33" so it’s medium-hard. For this project, 10 kg of silicone was used. But before the silicone was poured inside the vessel an anti-sticking spray ("Achem SG-1008S") was used to spray a thin layer onto the bust. This is important so that the silicone doesn't stick afterward. After the spray was dried (it forms a waxy layer) a thin layer of the silicone was applied to the whole bust by using a small brush. This way it's ensured that there are no air bubbles trapped in between the plaster and the silicone. Then the rest of the silicone was poured inside the vessel. The following figure shows the silicone mold once it was cured.

<img src="https://user-images.githubusercontent.com/44895720/90312479-14803b00-df05-11ea-9f67-daea6af736c2.jpeg" width="500">

The next step was to unmold the head. In some parts, the head was a bit difficult to get out of the mold but in the end, it worked out well. The next figure shows the finished silicone mold.

<img src="https://user-images.githubusercontent.com/44895720/90312531-88bade80-df05-11ea-94a2-e023685e7bfa.jpeg" width="500">

## 2. Capacitive Sensing

This section will describe how the capacitive sensing capabilities were built, what materials were used, etc.

For this project, an "ARDUINO UNO REV3" which was powered through a Laptop via USB was used for programming. There’s a "Capacitive Sensing Library" for Arduino that can turn two or more Arduino pins into a capacitive sensor with the help of copper foils and resistors (https://playground.arduino.cc/Main/CapacitiveSensor/). These sensors can then sense the electrical capacitance of the human body. There are also dedicated sensors available that can be bought but the approach with the copper foils offers two important advantages:

* They are much slimmer and their size can be adjusted because one can just cut a sensor into the desired size and shape.
* They offer more customization on the software side so that the sensors can be programmed exactly the way which is best suited for the particular project.

The following figure shows how such pieces of copper foil can look like.

<img src="https://user-images.githubusercontent.com/44895720/90331894-82d20580-dfb8-11ea-8128-0a94e7270aa5.jpeg" width="500">

Because the capacitive signal can pass through various types of materials (including plastic, wood, or in this case plaster) the sensors are invisible in the end because they are placed behind the respective material. To detect swipe gestures the authors decided to use two sensors (in this example called "A" and "B"). When a swipe from "A" to "B" should be detected one just has to register the times when the sensors are touched and see if the time difference is under a certain threshold which one has to define beforehand.

Through some basic tests, we found out that an additional copper foil layer (which was connected to a ground pin of the Arduino) should be placed behind the sensors to improve the signal quality. To avoid a short circuit a layer of hot glue which acts as an isolating layer was used between the two layers. However, still, the system was too unstable and the signal-to-noise ratio was too bad because of the capacitive signal inadvertently spreading throughout the plaster even when it's fully dried. This means that Arduino's built-in capacitive sensing capabilities aren't suited for this project.

This is why the authors decided to use a different solution. The solution that was found was to use the "Adafruit 12-Key Capacitive Touch Sensor Breakout - MPR121" (https://learn.adafruit.com/adafruit-mpr121-12-key-capacitive-touch-sensor-breakout-tutorial). As the name suggests it supports up to 12 individual touchpads. The MPR121 chip handles filtering and can also be configured to be more or less sensible. The following figure shows how this touch chip is connected to the Arduino (there are two sensors connected to the chip in this example).

<img src="https://user-images.githubusercontent.com/44895720/90981707-e2a15100-e562-11ea-84c9-8a61558dca98.png" height="450">

To be able to program the chip there is the "Adafruit_MPR121" library that has to be installed. There is a basic program under "codes/capacitive_sensors_mpr121.ino" which outputs which sensor is touched and released in the serial monitor. The way it works is that this code keeps track of the 12 "bits" for each touch. The sensor measures the capacitance with "counts". There’s a baseline count number that depends on different factors like temperature or humidity. When the number changes a significant amount it's considered as a touch or release. To be precise, each sensor has two values: The aforementioned base value ("cap.baselineData(i)") and the current filtered data reading ("cap.filteredData(i)") (i goes from 0 to 11). When the current reading is within 12 counts of the baseline value it's considered that the sensor is not touched. When the reading is more than 12 counts smaller than the baseline value a touch is reported. There’s also the possibility to see if a particular sensor is touched, e.g., sensor #4: "if (cap.touched() & (1 « 4)) { do something }". There are also commands to get the baseline and filtered data of each sensor ("filteredData(sensornumber);" respectively "baselineData(sensornumber);") so the thresholds (which are 12 in the default setting) can be adjusted to fit the particular project.

Here are some general tips on working with capacitive sensors that the authors found out:

* The thinner the outer plaster layer that covers the sensors is, the better (the thickness shouldn't exceed 5 mm).
* The sensors should be placed as far apart from each other as possible.
* The sensors shouldn't be too tiny. Making them a little bit bigger can already make a huge difference. Coin-shaped sensors with a diameter of about 2.5 cm worked well.
* It isn't advisable to bundle a bunch of cables together with cable ties. The sensors are still usable but the value differences when the sensors are touched versus when they aren't touched become smaller then.
* It's very important to make the connection of the sensors to the MPR121 as direct as possible. The authors tried to use a D-sub connector to connect the sensors of the final bust to the MPR121 to tidy the connections up a bit but that didn't turn out well: The value range (untouched vs. touched) became way smaller then. Apparently, the connector reduces the capacitive signal. When using a D-sub connector between the MPR121 and the Arduino (as the authors also did) that's no problem.

Finally, we added a calibration phase at the start to further improve our results. The way this works is the following: When the Arduino program is started there's a calibration phase that lasts 20 seconds and in which every sensor from the bust is touched multiple times. The software is programmed so that it registers the minimum and maximum values of each sensor during that calibration phase. Afterward, when the calibration is done, and the program is in its normal state, a touch is registered when the value goes down into the range of "minimum value + x" where "x" is a value that has to be defined beforehand. When this value would be 0 that could be problematic because when a sensor was pressed very hard during the calibration phase and isn't pressed so hard again later on during the normal interaction phase, a touch would never be registered. The values that the authors used for this project range from 1-4 as can be seen in the final code. Once the calibration is done, the program can run normally for as long as desired. Just when the bust is moved to a completely different location which might, e.g., be way more humid the calibration should be done again because the readings can be different in this new location.

## 3. Output Modalities

The next task was figuring out how exactly the output should be presented once a potential visitor interacts with the bust. The authors decided to use two different forms of output:

* audio
* a display

The next step was to formulate some short texts for each of the five different scenarios (information about the foundation of the museum, information regarding the person's wife, etc.). Then these texts were recorded as audio recordings (.mp3 format) and stored onto a microSD card. To play these recordings using an Arduino the "DFPlayer Mini MP3 Player For Arduino" was used (https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299). This is a small MP3 module with a built-in microSD card reader that can output directly to a speaker. It even features a digital-to-analog converter (DAC). Before being able to use it it's necessary to install the "DFRobotDFPlayerMini" library for the Arduino. As a speaker, a simple 8 Ω speaker was used. The following figure shows a wiring diagram for a setup that involves the MP3 player and a speaker.

<img src="https://user-images.githubusercontent.com/44895720/91063335-26ad5800-e62e-11ea-9389-5be6e2bb1850.png" width="500">

Before trying the audio output in combination with the bust a basic test was performed with two capacitive sensors and two different audio recordings. Depending on which sensor was touched a different recording should play. The code to implement this functionality can be found under "codes/capacitive_audio.ino". Some important notes are that the microSD card has to be formatted in either the FAT16 or the FAT32 format and that the .mp3 files have to be placed in a folder called "MP3" which has to be put in the root directory. The .mp3 files themselves have to be named "0001.mp3", "0002.mp3", etc.

As a display, the authors chose an e-paper display. E-paper displays (like their name already implies) mimic the appearance of ink on paper so they have a rather analog look which is fitting for our use case. Conventional displays, e.g., LCDs are backlit and emit light whereas e-paper displays reflect light which makes them similar to paper. The particular display that was used for this paper is the "Waveshare 5.83inch e-Paper HAT" (https://www.waveshare.com/wiki/5.83inch_e-Paper_HAT). This panel is a 600 x 448-pixel two-color display that is capable of displaying black and white content. This raw panel additionally requires a driver board. The one that was used for this paper is the "Waveshare E-Paper Shield" (https://www.waveshare.com/w/upload/c/c8/E-Paper_Shield_User_Manual_en.pdf). This is a universal driver board that can be used to connect an e-paper raw panel to an Arduino. The display is connected to the shield via a ribbon cable which is included in delivery and the shield can simply be put on top of the Arduino UNO. The only issue is that the shield blocks all of the Arduino pins. This is why the "PROTO SHIELD REV3 (UNO SIZE)" was used as an "intermediate ceiling". The e-paper shield also has an external SPI RAM chip onboard which makes it possible to use it for microSD card reading and writing. The first step to being able to program this display is to download the demo code from the website (demo codes for all Waveshare e-paper displays can be found under https://github.com/waveshare/e-Paper) and then to import the "EPD" library. The microSD card needs to be formatted in the FAT format and the images need to be monochrome BMPs. As mentioned previously, five audio files were recorded which are played when the user interacts with different parts of the bust. Alongside this auditory modality, the plan was to additionally display one image each which goes together with the respective audio recording and which is displayed on the e-paper display. So five simple, fitting images were drawn using an iPad Pro, an Apple Pencil, and the iPadOS app "Paper". The images were exported (as .pngs), scaled to 600 x 448 pixel, and saved as .bmps. A website was used to convert these images into monochrome .bmp images (https://online-converting.com/image/convert2bmp/). Lastly, the images were saved onto a microSD card which was inserted into the e-paper shield.

## 4. Creation of the Prototype of the Interactive Bust

In this chapter, it will be described step-by-step how the final prototype of the interactive bust was created. Alongside, some tips will be shared as well as things to pay attention to. The final code for this prototype can be found under "codes/interactive_bust.ino".

### Step 1: Marking the positions of the sensors

This first step was sort of a pre-step to the actual creation of the bust. Depending on the specific needs for the project it's not absolutely necessary. When the exact placement of the sensors is not crucial one can omit this step. But in this particular project, the exact placement is very important. The reason why it's then necessary to mark the positions of the sensors beforehand is the following: Once the first layer of plaster is poured the small details in the silicone mold can't be properly seen anymore. The solution that was found involves a laser pointer mounted on a wooden plank. A wooden plank was cut and a hole was drilled inside of it so that a laser pointer could fit into it. Then this plank was placed on top of the silicone mold and moved around to the different spots so that at each position the laser pointer pointed at the exact position where a copper pad should be. Then the respective positions were marked on the plank and also on the silicone mold. This can be seen in the following figure.

<img src="https://user-images.githubusercontent.com/44895720/90980971-a66bf180-e55e-11ea-9e1a-79c73a982089.jpeg" height="450">

### Step 2: Preparation of the silicone mold

The next step was to prepare the silicone mold. The product that was used is called "HINRISID Oberflächenentspanner auf Tensidbasis" which is a leveling agent thus improving the surface of the bust. When using it the surface of the bust is a bit smoother because the plaster is dispersed more evenly. To apply the product it's best when it is mixed with a little bit of water and then applied onto the inside of the silicone mold using a brush. Then it has to dry for about 2 minutes and then one has to fan it dry. The authors used some kitchen roll to dab dry the parts which had a bit of excess liquid.

### Step 3: Applying the first layer of plaster

Then some plaster was mixed with warm water (in a ratio of 1.5:1). It's important to pour the plaster into the water and not vice versa to avoid any clumping. Then it has to be mixed properly until smooth.

**Notes about different types of plaster:** Something very important is that there are multiple types of plaster available. While a basic plaster which can be found in any hardware store works it doesn't yield the best results. Using gypseous alabaster brings three advantages: Firstly, it's easier to work with it since it cures more slowly. So once the plaster and the water are mixed the amount of time when the plaster is viscous is much longer. Secondly, it becomes much harder. This helps with the structural integrity of the bust. And lastly, the color is different: While the basic plaster is a bit grayish, the gypseous alabaster is more white.

Then the plaster was applied in a thin layer of about 1-2 mm thickness. It helps when the mold is tilted around a bit and when a brush is used to get the plaster into every nook and cranny of the silicone mold. It's very important that the first layer of plaster doesn't exhibit any holes.

**Notes about the thickness of the plaster layer:** It makes a huge difference how thick this first layer is. It's important to make it as thin as possible because that helps with the capacitive sensing but when making it too thin the risk is that parts of the bust break off during the unmolding process. When making the layer too thick the capacitance can't properly get through the layer later on. A thickness of about 1-2 mm works best.

Before the plaster dried the authors used some gauze and pressed it into the parts of the nose and lips. The reason is that the first prototypes exhibited some structural weaknesses in these areas and the gauze helps to add some more stability to these parts. There are a few things to note:
* The gauze must not be pressed down into the plaster too hard. Otherwise, it will be seen on the outside of the bust later on. It just has to be gently pressed into the viscous plaster.
* It's possible to also use other materials that give structural strength (like polymer fibers or a fiberglass cloth) but the gauze soaks up the plaster a bit which is why this material is superior to the other options since this means that it does a better job of giving additional strength to the outer layer.

Then the plaster has to dry. It's best to let the plaster dry at least overnight. The result of this step can be seen in the following figure.

<img src="https://user-images.githubusercontent.com/44895720/90979216-f3969600-e553-11ea-9ac6-b886fd5458d9.jpg" height="450">

### Step 4: Adding the sensors

The next step was to add the sensors. The authors decided to use copper foil for this. The foil came pre-glued onto paper so it could be cut, peeled off, and then glued onto the dried plaster. To be sure that the sensors are placed at the exact locations where they should be the trick with the laser pointer was used. It's important that the sensors aren't too small. Otherwise, the capacitive sensing will get problematic later on. Coin-shaped sensors with a diameter of about 2.5 cm worked well. It has to be ensured that the sensors have good contact with the plaster. This can be a bit challenging on very curved surfaces like on the nose but a few cuts with a scissor help the copper foil to adhere to the surface of the plaster. The sensors on the hair part are deliberately bigger because there the goal was that it doesn't matter where exactly the user touches the bust. Lastly, some wires needed to be soldered to the sensors. The following figure shows the result of this step.

<img src="https://user-images.githubusercontent.com/44895720/90979719-50e01680-e557-11ea-9ca2-efe19886f93d.jpg" height="450">

### Step 5: Adding the hot glue

Then followed the isolating layer. The authors chose to use hot glue for this purpose. The hot glue doesn't need to be applied everywhere but just on a rectangle that covers all of the sensors. During this step, one has to pay attention that every part of the sensors is covered so that there are no holes in the hot glue layer. Otherwise, there can be a short circuit later on due to the grounding layer (that now follows) being in contact with the sensors. The result of this step can be seen in the following figure.

<img src="https://user-images.githubusercontent.com/44895720/90979736-6f461200-e557-11ea-9d59-ec8756725c11.jpg" height="450">

### Step 6: Adding the grounding layer

The next step was to add the grounding layer. Again, the authors chose copper foil for that. The grounding layer is important to improve the results of the capacitive sensing. As with the hot glue, the grounding foil was applied in a rectangle that covers all sensors. Then a cable needs to be soldered to this layer. The next figure shows the result of this step. Note: The picture was taken before this wire was added.

<img src="https://user-images.githubusercontent.com/44895720/90979905-4eca8780-e558-11ea-9270-7024f56e728f.jpg" height="450">

### Step 7: Adding the "Moltofill"

This step involved applying a layer of Moltofill. The reason is the following: Liquid plaster doesn't combine well with already cured plaster and it also expands when drying so the problem is that the second plaster layer might cause the first one to crack a bit because it expands a little bit. So the idea is to apply a thin layer of Moltofill before applying the second plaster layer. The Moltofill should be slightly dried but not all the way before the second plaster layer is poured so that it combines well. The following figure shows the result after adding the Moltofill and letting it slightly dry.

<img src="https://user-images.githubusercontent.com/44895720/90980025-1d9e8700-e559-11ea-8cb6-1be0be3214f3.jpg" height="450">

### Step 8: Adding the last layer of plaster

The last step regarding the creation of the bust itself was to add the second layer of plaster. This layer can basically be as thick as one wants to give enough structural stability to the bust. To make the process a bit easier a wooden frame in the shape of the contour of the silicone mold was used which was placed on top of the mold and which had overlapping edges to the inside (about 1 cm) as a guide to achieve even plaster thickness. The next figure shows the result.

<img src="https://user-images.githubusercontent.com/44895720/90980130-cb119a80-e559-11ea-85d0-3b55a79fdc10.jpg" height="450">

The following figure shows a schematic drawing of all of the different layers. For this example, just two sensors are shown. The left one has a layer of gauze under it to give more stability while the right one is just left as is (to depict the two approaches that are present in the bust). The gaps in the sketch are only for visibility purposes.

<img src="https://user-images.githubusercontent.com/77795295/107211101-606bf280-6a05-11eb-99d9-09d11e9fd51a.png" width="500">

### Step 9: Unmolding

Before unmolding the bust it's best to let it cure for a couple of days. Even then the outside is slightly damp. This is why it's advised to give the bust another 1-2 days after unmolding to dry out more. The resulting bust can be seen in the following figure.

<img src="https://user-images.githubusercontent.com/44895720/90980165-09a75500-e55a-11ea-8721-b6d557fc9c75.jpg" height="450">

### Step 10: Marking the interactive parts on the outside of the bust

This was an optional step but the authors marked the interactive parts on the outside of the bust because it was important in the scope of the paper. The authors decided to use watercolors which were applied onto the outside of the bust using a brush. The only thing one has to pay attention to is that the brush is not too wet when applying the watercolors. Otherwise, the colors run off, ruining the bust. A skin-colored watercolor was used for the interactive parts on the face while a brown color was used for the hair part.

The following figure shows the resulting bust.

<img src="https://user-images.githubusercontent.com/77795295/107773572-2751bc00-6d3e-11eb-9994-1b8d03c3e3c5.jpeg" width="500">

### Step 11: Mounting the bust

This was another optional step but for this project, the authors decided to mount the bust onto a box to properly display it. The first goal was that users can comfortably interact with the bust and the second goal was that all of the electronic parts could be hidden so that users don’t see any cables, etc. hanging around. The solution was to build a wooden box which can be seen in the following figure.

<img src="https://user-images.githubusercontent.com/77795295/108492196-258b7980-72a5-11eb-9ec3-75ecc3e3c553.jpeg" height="450">

The idea was that the bust is placed on the right-hand side while all of the cables are fed through one of the two holes on the right. Through a wooden cross that was attached to the backside of the bust with tile adhesive (see the following picture) it could be mounted onto the slanted board using two small holes that were drilled into the cross and the slanted board and with the help of a screw and a nut.

<img src="https://user-images.githubusercontent.com/77795295/107773963-b1018980-6d3e-11eb-8281-c11caa6da439.jpg" height="450">

On the left-hand side of the box the e-paper display was placed and the cables could be routed to the backside of the box through a slit that was cut into the slanted board. This made it possible to neatly display the bust for users who can then easily interact with it.

The following figure shows the backside of the wooden box with a few annotations regarding the different parts of the setup.

<img src="https://user-images.githubusercontent.com/77795295/107774056-d0001b80-6d3e-11eb-97b1-6b6a262a279e.jpg" width="500">

The finished setup can be seen in the next figure.

<img src="https://user-images.githubusercontent.com/77795295/108492973-0e995700-72a6-11eb-9ec8-967edc477b2e.jpeg" width="500">

## 5. Limitations

Our prototype was a first step toward an interactive plaster bust, including the material exploration and our implementation requiring further iterations. We discovered that for very curved surfaces like the lips it sometimes took a few tries before the system registered an interaction. Furthermore, our sensors have to be embedded between multiple layers to assure a stable signal and structural integrity, which requires a replacement of the whole bust when one sensor breaks. Further exploration would be necessary to improve our technical solution and to make the hardware maintainable.
