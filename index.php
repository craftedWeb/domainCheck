<?php
include ('assets/includes/header.php');
<div class='domainCheck'>
  <form class='domainCheck' method='post' action='domaincheck.php'>
    <input type='text' id='domain-input' name='domain' placeholder='Gewenste domeinnaam:'>
    <input type='submit' id='check-button' name='submit' value='Controleer beschikbaarheid'>
  </form>
  <p id='availability-message'></p>
</div>
include ('assets/includes/footer.php);  
?>
