////  Page-scoped globals  ////


// Counters
var rocketIdx = 1;
var asteroidIdx = 1;
var destroyedAsteroids = 0;
var accuracy = 0;
var formIndex = 0;
var shieldIndex;
var shieldOn = 0;
var splashOn=1;
var startLives, startApS, startSpA;
var audioExp = new Audio('audio/explode.wav');
var audioOver = new Audio('audio/gameOver.wav');
var audioIntro = new Audio('audio/intro.mp3');
var audioRoc = new Audio('audio/rocket.wav');

// Size Constants
var MAX_ASTEROID_SIZE = 50;
var MIN_ASTEROID_SIZE = 15;
var ASTEROID_SPEED = 5;
var ROCKET_SPEED = 10;
var SHIP_SPEED = 25;
var OBJECT_REFRESH_RATE = 50;  //ms
var SCORE_UNIT = 100;  // scoring is in 100-point units

// Size vars
var maxShipPosX, maxShipPosY;

// Input vars
var lives = 3, ApS = 1, SpA = 10;
 
// Global Window Handles (gwh__)
var gwhGame, gwhOver, gwhStatus, gwhScore, gwhSettingStatus = 1, gwhSplash;

// Global Object Handles
var ship;

/*
 * This is a handy little container trick: use objects as constants to collect
 * vals for easier (and more understandable) reference to later.
 */
var KEYS = {
  left: 37,
  up: 38,
  right: 39,
  down: 40,
  shift: 16,
  spacebar: 32
}


// Main
$(document).ready( function() {
  console.log("Ready!");

  // Set global handles (now that the page is loaded)
  gwhGame = $('.game-window');
  gwhOver = $('.game-over');
  gwhStatus = $('.status-window');
  gwhScore = $('#score-box');
  gwhAccuracy = $('#accuracy-box');
  gwhSplash = $('.splash');
  ship = $('#enterprise');  // set the global ship handle

  // Set global positions
  maxShipPosX = gwhGame.width() - ship.width();
  maxShipPosY = gwhGame.height() - ship.height();

  $(window).keydown(keydownRouter);

  if(gup("life") != null){

      if(gup("life") > 10){
        lives = 10;
      }else if(gup("life") < 1){
        lives = 3;
      }else{
        lives = gup("life");
      }

    document.getElementById("input-lives-box").value = lives;
  }

  if(gup("itemRate") != null){

      if(gup("itemRate") > 100){
        SpA = 100;
      }else if(gup("itemRate") < 5){
        SpA = 5;
      }else{
        SpA = gup("itemRate");
      }

      document.getElementById("input-shields-box").value = lives;
  }


  audioIntro.play();



 
});

function start_game(){
  // Periodically check for collisions (instead of checking every position-update)
  //Check gup
 startLives = lives;
  startSpA = SpA;
  startApS = ApS;
  splashOn=0;
 gwhSplash.hide();
 audioIntro.pause();
 audioIntro.load();
 for(var i = 1; i < lives; ++i){

    $("<img src='img/fighter.png' height='10px'/>").insertBefore(".lifebox-end");

  }

  setInterval( function() {
    checkCollisions();  // Remove elements if there are collisions
  }, 100);

  var asteroidRate = (1000/ApS)  +   (1000/ApS)*(Math.random()/2);

  console.log((1/asteroidRate)*1000);

  setInterval( function() {
    if(splashOn == 0){createAsteroid();}
  }, asteroidRate);


}



function keydownRouter(e) {
  switch (e.which) {
    case KEYS.shift:
      break;
    case KEYS.spacebar:
      fireRocket();
      break;
    case KEYS.left:
    case KEYS.right:
    case KEYS.up:
    case KEYS.down:
      moveShip(e.which);
      break;
    default:
      console.log("Invalid input!");
  }
}

// Check for any collisions and remove the appropriate object if needed
function checkCollisions() {
  // First, check for rocket-asteroid checkCollisions
  /* NOTE: We dont use a global handle here because we need to refresh this
   * list of elements when we make the reference.
   */
  $('.rocket').each( function() {
    var curRocket = $(this);  // define a local handle for this rocket
    $('.asteroid').each( function() {
      var curAsteroid = $(this);  // define a local handle for this asteroid

      // For each rocket and asteroid, check for collisions
      if (isColliding(curRocket,curAsteroid)) {
        // If a rocket and asteroid collide, destroy both
        curRocket.remove();
        curAsteroid.remove();

        // Score points for hitting an asteroid! Smaller asteroid --> higher score
        //Increment destroyed ateroids counter
        var points = Math.ceil(MAX_ASTEROID_SIZE-curAsteroid.width()) * SCORE_UNIT;
        destroyedAsteroids++;
        if(destroyedAsteroids % SpA == 0){

          if(shieldOn != 1){
            //modify asteroid code to create a new "type" of asteroid -- a shield
            createAsteroid();
            shieldIndex = "a-" + (asteroidIdx-1);

            document.getElementById(shieldIndex).innerHTML = "<img src='img/shield.png' height='" + 100 + "'/>";
            $('#'+shieldIndex).css('width', 100+"px");
            $('#'+shieldIndex).css('height', 100+"px");
          }
        }


        // Update the visible score
        // Update the visible accuracy
        gwhScore.html(parseInt($('#score-box').html()) + points);
        // Update the visible accuracy
        accuracy = (destroyedAsteroids/(rocketIdx - 1))*100;
        accuracy = accuracy.toFixed(0);
        gwhAccuracy.html(accuracy + '%');
      }
    });
  });


  // Next, check for asteroid-ship interactions
  $('.asteroid').each( function() {
    var curAsteroid = $(this);
    if (isColliding(curAsteroid, ship)) {

      if(this.getAttribute('id') == shieldIndex){
        shieldOn = 1;
        $("#"+shieldIndex).remove();
        var imgStr = "<img id='S' src='img/shield.png' height='" + 100 + "'/>";
        ship.append(imgStr);
        $('#S').css('position', 'fixed');
        $('#S').css('top', ship.css('top'));
        $('#S').css('left', ship.css('left'));
        $('#S').css('z-index', '3');

      }else if(shieldOn == 1){
        $('#S').remove();
        shieldOn = 0;
        shieldIndex = null;
        curAsteroid.remove();
      }else{

    
        curAsteroid.remove();
        var imgStr = "<img id='exp' src='img/explosion.png' height='" + 100 + "'/>";
        ship.append(imgStr);
        $('#exp').css('z-index', '3');
        $('#exp').css('position', 'fixed');
        $('#exp').css('top', ship.css('top'));
        $('#exp').css('left', ship.css('left'));
        $('.rocket').remove();  // remove all rockets
        $('.asteroid').remove();  // remove all asteroids
        //center ship
        $('.ship').css('top', '530px');
        $('.ship').css('left', '122px');
        audioExp.play();
        setTimeout(function(){ 

          
         
          $('#exp').remove(); 
          audioExp.pause();
          audioExp.load();

          --lives;
          //clear remaining live images.
          document.getElementById('lifebox').innerHTML = "<div class= 'lifebox-end'></div>";

          if(lives == 0){
            // Hide primary windows
            gwhGame.hide();
            gwhStatus.hide();
            ship.hide();
            // Show "Game Over" screen
            audioOver.play();
            document.getElementById('final-score').innerHTML = document.getElementById('score-box').innerHTML;
            gwhOver.show();
            splashOn=1;
          }else{
            for(var i = 1; i < lives; ++i){

              $("<img src='img/fighter.png' height='10px'/>").insertBefore(".lifebox-end");

            }
          }
        }, 1000);
      }
      
        
      
    }
  });
}

// Check if two objects are colliding
function isColliding(o1, o2) {
  // Define input direction mappings for easier referencing
  o1D = { 'left': parseInt(o1.css('left')),
          'right': parseInt(o1.css('left')) + o1.width(),
          'top': parseInt(o1.css('top')),
          'bottom': parseInt(o1.css('top')) + o1.height()
        };
  o2D = { 'left': parseInt(o2.css('left')),
          'right': parseInt(o2.css('left')) + o2.width(),
          'top': parseInt(o2.css('top')),
          'bottom': parseInt(o2.css('top')) + o1.height()
        };

  // If horizontally overlapping...
  if ( (o1D.left < o2D.left && o1D.right > o2D.left) ||
       (o1D.left < o2D.right && o1D.right > o2D.right) ||
       (o1D.left < o2D.right && o1D.right > o2D.left) ) {

    if ( (o1D.top > o2D.top && o1D.top < o2D.bottom) ||
         (o1D.top < o2D.top && o1D.top > o2D.bottom) ||
         (o1D.top > o2D.top && o1D.bottom < o2D.bottom) ) {

      // Collision!
      return true;
    }
  }
  return false;
}

// Return a string corresponding to a random HEX color code
function getRandomColor() {
  // Return a random color. Note that we don't check to make sure the color does not match the background
  return '#' + (Math.random()*0xFFFFFF<<0).toString(16);
}

// Handle asteroid creation events
function createAsteroid() {
  console.log('Spawning asteroid...');

  // NOTE: source - http://www.clipartlord.com/wp-content/uploads/2016/04/aestroid.png
  var asteroidDivStr = "<div id='a-" + asteroidIdx + "' class='asteroid'></div>"
  // Add the rocket to the screen
  gwhGame.append(asteroidDivStr);
  // Create and asteroid handle based on newest index
  var curAsteroid = $('#a-'+asteroidIdx);

  asteroidIdx++;  // update the index to maintain uniqueness next time

  // Set size of the asteroid (semi-randomized)
  var astrSize = MIN_ASTEROID_SIZE + (Math.random() * (MAX_ASTEROID_SIZE - MIN_ASTEROID_SIZE));
  curAsteroid.css('width', astrSize+"px");
  curAsteroid.css('height', astrSize+"px");
  curAsteroid.append("<img src='img/asteroid.png' height='" + astrSize + "'/>")

  /* NOTE: This position calculation has been moved lower since verD -- this
  **       allows us to adjust position more appropriately.
  **/
  // Pick a random starting position within the game window
  var startingPosition = Math.random() * (gwhGame.width()-astrSize);  // Using 50px as the size of the asteroid (since no instance exists yet)

  // Set the instance-specific properties
  curAsteroid.css('left', startingPosition+"px");

  // Make the asteroids fall towards the bottom
  setInterval( function() {

    curAsteroid.css('top', parseInt(curAsteroid.css('top'))+ASTEROID_SPEED);
    // Check to see if the asteroid has left the game/viewing window
    if (parseInt(curAsteroid.css('top')) > (gwhGame.height() - curAsteroid.height())) {
      curAsteroid.remove();
    }
  }, OBJECT_REFRESH_RATE);
}

// Handle "fire" [rocket] events
function fireRocket() {
  console.log('Firing rocket...');
  audioRoc.play();

  // NOTE: source - https://www.raspberrypi.org/learning/microbit-game-controller/images/missile.png
  var rocketDivStr = "<div id='r-" + rocketIdx + "' class='rocket'><img src='img/rocket.png'/></div>";
  // Add the rocket to the screen
  gwhGame.append(rocketDivStr);
  // Create and rocket handle based on newest index
  var curRocket = $('#r-'+rocketIdx);
  rocketIdx++;  // update the index to maintain uniqueness next time

  // Set vertical position
  curRocket.css('top', ship.css('top'));
  // Set horizontal position
  var rxPos = parseInt(ship.css('left')) + (ship.width()/2);  // In order to center the rocket, shift by half the div size (recall: origin [0,0] is top-left of div)
  curRocket.css('left', rxPos+"px");

  // Create movement update handler
  setInterval( function() {
    curRocket.css('top', parseInt(curRocket.css('top'))-ROCKET_SPEED);
    // Check to see if the rocket has left the game/viewing window
    if (parseInt(curRocket.css('top')) < curRocket.height()) {
      //curRocket.hide();
      // Update the visible accuracy
      accuracy = (destroyedAsteroids/(rocketIdx - 1))*100;
      accuracy = accuracy.toFixed(0);
      gwhAccuracy.html(accuracy + '%');
      curRocket.remove();
    }
  }, OBJECT_REFRESH_RATE);
}

// Handle ship movement events
function moveShip(arrow) {
  switch (arrow) {
    case KEYS.left:  // left arrow
      var newPos = parseInt(ship.css('left'))-SHIP_SPEED;
      if (newPos < 0) {
        newPos = 0;
      }
      ship.css('left', newPos);
      if(shieldOn == 1) {$('#S').css('left', newPos);}
      
    break;
    case KEYS.right:  // right arrow
      var newPos = parseInt(ship.css('left'))+SHIP_SPEED;
      if (newPos > maxShipPosX) {
        newPos = maxShipPosX;
      }
      ship.css('left', newPos);
      if(shieldOn == 1) {$('#S').css('left', newPos);}
    break;
    case KEYS.up:  // up arrow
      var newPos = parseInt(ship.css('top'))-SHIP_SPEED;
      if (newPos < 0) {
        newPos = 0;
      }
      ship.css('top', newPos);
      if(shieldOn == 1) {$('#S').css('top', newPos);}
    break;
    case KEYS.down:  // down arrow
      var newPos = parseInt(ship.css('top'))+SHIP_SPEED;
      if (newPos > maxShipPosY) {
        newPos = maxShipPosY;
      }
      ship.css('top', newPos);
      if(shieldOn == 1) {$('#S').css('top', newPos);}
    break;
  }
}

//Bring up settings panel
function settingClick(){
  if(gwhSettingStatus == 0){
    $('.panel').css("display", "none");
    gwhSettingStatus =1;
    document.getElementById("setting-button-text").innerHTML = "Open Setting Panel";
  }else  if(gwhSettingStatus == 1){
    $('.panel').css("display", "block");
    gwhSettingStatus =0;
    document.getElementById("setting-button-text").innerHTML = "Close Setting Panel";

  }

}

function update(){

  lives = document.getElementById("input-lives-box").value;

  ApS = document.getElementById("input-asteroids-box").value;
  
  SpA = document.getElementById("input-shields-box").value;

  if(lives < 0 || lives == ""){
    lives = startLives;
  }

  if(ApS == ""){
    ApS = startApS;
  }
  if(SpA == ""){
    SpA = startSpA;
  }
  


  if(ApS < .2){
    alert("Error: Please input an Asteroid per Second Ratio between .2 and 4");
    ApS = .2;
  }

  if(ApS > 4){
    alert("Error: Please input an Asteroid per Second Ratio between .2 and 4");
    ApS = 4;
  }

  //clear all asteroids and rockets.
  $('.rocket').remove();  
  $('.asteroid').remove(); 

  //clear remaining live images.
  document.getElementById('lifebox').innerHTML = "<div class= 'lifebox-end'></div>";



  //center ship
  $('.ship').css('top', '530px');
  $('.ship').css('left', '122px');
  
 // Counters
  rocketIdx = 1;
  asteroidIdx = 1;
  destroyedAsteroids = 0;
  accuracy = 0;
  formIndex = 0;

  if(shieldOn==1){
    $('#S').remove();
    shieldIndex=null;
  }

  shieldOn=0;

  gwhScore.html(0);
  gwhAccuracy.html(0);
  settingClick();

  //start_game();



}

function new_game(){

  gwhOver.hide();
  gwhSplash.show();
  gwhGame.show();
  gwhStatus.show();
  ship.show();
  update();
}

// function for getting URL parameters
function gup(name) {
    name = name.replace(/[\[]/,"\\\[").replace(/[\]]/,"\\\]");
    var regexS = "[\\?&]"+name+"=([^&#]*)";
    var regex = new RegExp(regexS);
    var results = regex.exec(window.location.href);
    if(results == null)
      return null;
    else
      return unescape(results[1]);
}


