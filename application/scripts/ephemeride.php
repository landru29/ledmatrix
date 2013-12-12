#!/usr/bin/env php

<?php

// font[contains(@class, "Fete")]

$siteWeb = "http://www.ephemeride.com";
$href = "/ephemeride/ephemeride/11/";

$prenoms = array();
//$page = getPage($siteWeb . $href);
$page = file_get_contents_utf8($siteWeb . $href);
$matches = array();
if (preg_match_all('/<font class="Fete[\w]*">([^<]*)<\/font>/', $page, $matches) > 0) {
    if (count($matches) == 2) {
        $prenoms = $matches[1];
    }
}
function file_get_contents_utf8($fn) { 
    $content = file_get_contents($fn); 
    return mb_convert_encoding($content, 'UTF-8', 
        mb_detect_encoding($content, 'UTF-8, ISO-8859-1', true)); 
} 
function wd_remove_accents($str, $charset='UTF-8')
{
    //$str = html_entity_decode($str, ENT_NOQUOTES|ENT_HTML401, $charset);
    echo $str, PHP_EOL, PHP_EOL;
    
    $str = preg_replace('#&([A-za-z])(?:acute|cedil|circ|grave|orn|ring|slash|th|tilde|uml);#', '\1', $str);
    $str = preg_replace('#&([A-za-z]{2})(?:lig);#', '\1', $str); // pour les ligatures e.g. '&oelig;'
    $str = preg_replace('#&[^;]+;#', '', $str); // supprime les autres caractères
    echo $str, PHP_EOL, PHP_EOL;
    
    return $str;
}
//echo 'wd_remove_accents', PHP_EOL;
//echo wd_remove_accents(file_get_contents($siteWeb . $href), 'ISO-8859-1');

function getPage($url)
{
  $page = mb_convert_encoding(file_get_contents($url), 'UTF-8', 'ISO-8859-1');
  $page = mb_convert_encoding($page, 'UTF-8', 'HTML-ENTITIES');
  return $page;
}

$accents = array(
    'À','Á','Â','Ã','Ä','Å',
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
    $prenoms[$key] = wd_remove_accents($prenom);
}
if (!empty($prenoms)) {
    $command = 'sudo /usr/local/bin/newspaper -m "Bonne fete aux: ' . implode(', ', $prenoms) . '"';
    exec($command, $result);
    echo $command, PHP_EOL;
}

?>
