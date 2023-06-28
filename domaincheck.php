<?php
require_once("assets/classes/TransipAPI.inc");

$domainStatuses = [
	"inyouraccount" => "geregistreerd",
	"unavailable" => "niet beschikbaar",
	"notfree" => "geregistreerd",
	"free" => "beschikbaar",
	"internalpull" => "geregistreerdPull",
	"internalpush" => "geregistreerdPush"
];


$domainName = strtolower($_POST["domain"]);
$api = new TransipAPI();

try {
    $response = $api->checkDomain($domainName);

    echo "<pre style='display: none' class='dev'>" . print_r($response, true) . "</pre>";

    echo "<p>Gecontroleerde domeinnaam: " . $response->domainName . "</p>";
    echo "<p>Status: " . $domainStatuses[$response->status] . "</p>";


    if ($response->status === "free") {
        echo "<form 
                method='post' 
                action=\"assets/functions/domains/bestelDomein.php/" . $response->domainName . "\">
                <input type='submit' value='Bestel domeinnaam'>
              </form>";
    }
    if ($response->status === "notfree" && in_array("transfer", $response->actions)) {
        echo "<form 
                method='post'
                action=\"assets/functions/domains/verhuisDomein.php/" . $response->domainName . "\">
                <input type='submit' value='Verhuis domeinnaam'>
              </form>";
    }
    if ($response->status === "internalpush" && in_array("internalpush", $response->actions)) {
        echo "<form
                method='post'    
                action=\"assets/functions/domains/verhuisDomeinPush.php/" . $response->domainName . "\">
                <input type='submit' value='Verhuis aanvragen'>
              </form>";
    }
    if ($response->status === "internalpull" && in_array("internalpull", $response->actions)) {
        echo "<form 
                method='post'
                action=\"assets/functions/domains/verhuisDomeinPull.php/" . $response->domainName . "\">
                <input type='submit' value='Verhuis aanvragen'>  
              </p>";
    }
}
catch (Exception $e) {
	echo $e->getMessage();
}
