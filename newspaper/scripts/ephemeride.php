#!/usr/bin/env php

<?php

// font[contains(@class, "Fete")]

$siteWeb = "http://www.ephemeride.com";
$href = "/ephemeride/ephemeride/11/fetes-prenoms-soleil-citation-lune-proverbe-dictons-du-jour.html";

$prenoms = array();
$page = getPage($siteWeb . $href);
$matches = array();
if (preg_match_all('/<font class="Fete[\w]*">([^<]*)<\/font>/', $page, $matches) > 0) {
    if (count($matches) == 2) {
        $prenoms = $matches[1];
    }
}
function getPage($url)
{
  $page = mb_convert_encoding(file_get_contents($url), 'UTF-8', 'ISO-8859-1');
  $page = mb_convert_encoding(file_get_contents($url), 'UTF-8', 'HTML-ENTITIES');
  return $page;
}

$accents = array(
    'À','Á','','Ã','Ä','Å',
    'Ç',
    'È','É','Ê','Ë',
    'Ì','Í','Î','Ï',
    'Ò','Ó','Ô','Õ','Ö',
    'Ù','Ú','Û','Ü',
    'Ý',
    'à','á','â','ã','ä','å',
    'ç',
    'è','é','ê','ë',
    'ì','í','î','ï',
    'ð','ò','ó','ô','õ','ö',
    'ù','ú','û','ü',
    'ý','ÿ'
);
$sans = array(
    'A','A','A','A','A','A',
    'C',
    'E','E','E','E',
    'I','I','I','I',
    'O','O','O','O','O',
    'U','U','U','U',
    'Y',
    'a','a','a','a','a','a',
    'c',
    'e','e','e','e',
    'i','i','i','i',
    'o','o','o','o','o','o',
    'u','u','u','u',
    'y','y'
);
foreach ($prenoms as $key => $prenom) {
    $prenoms[$key] = str_replace($accents, $sans, $prenom);
}
if (!empty($prenoms)) {
    $command = 'sudo /usr/local/bin/newspaper -m "Bonne fete aux: ' . implode(', ', $prenoms) . '"';
    //exec($command, $result);
    echo $command, PHP_EOL;
}

?>
