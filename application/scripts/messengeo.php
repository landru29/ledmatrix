#!/usr/bin/env php

<?php

$dateStart = date('d-m-Y', mktime(0,0,0,date('n'),1));
$dateEnd = date('d-m-Y', mktime(
  23,59,59,date('n'),
  date('d', mktime(0,0,0,date('n')+1,0))
));
$key = 'baseokey';
$env = 'intg-';
$url = "https://{$env}api.messengeo.net/rest/";
$resource = 'statistics';
$format = 'json';

$args = array(
    'action=read',
    'dateStart='. $dateStart,
    'dateEnd=' . $dateEnd,
    'media=EMAIL',
    'properties=total',
    'key=' . $key,
);

$uri = $url . $resource . '.' . $format . '?' . implode('&',  $args);
echo "Requesting " . $uri . "\n";

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $uri);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($ch, CURLOPT_HEADER, FALSE);
curl_setopt($ch, CURLOPT_HTTPHEADER, array("Accept: application/json"));
$response = curl_exec($ch);
$infos = curl_getinfo($ch);
curl_close($ch);

echo "Answer catched\n";

if (false === $response) {
  // gérer l'erreur
  echo "Error\n";
  file_put_contents('php://stderr', date('c') . ': Erreur de requête sur l\'API Messengeo.');
  file_put_contents('php://stderr', date('c') . ': ' . var_export($infos, true));
} else {
  echo "Success\n";
  $result = json_decode($response);
  var_dump($result);
  $total = $result->total;
  $command = 'ledmatrix "@ INTG: ' . $total . '"';
  echo "Launching " . $command . "\n";
  //$command = 'sudo /usr/local/bin/newspaper -m "@ INTG: ' . $total . '"';
  exec($command, $result);
}
?>
