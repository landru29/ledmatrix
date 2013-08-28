<?php

$dateStart = mktime(0,0,0,date('n'),1);
$dateEnd = mktime(
  23,59,59,date('n'), 
  mktime(0,0,0,date('n')+1,0)
);

$key = 'baseokey';
$env = 'intg-';
$url = "https://{$env}api.messengeo.net/rest/";
$resource = 'statistics';
$format = 'json';

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, $url . $resource . '.' . $format);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
curl_setopt($ch, CURLOPT_HEADER, FALSE);
curl_setopt($ch, CURLOPT_POSTFIELDS, "{\"action\":\"read\",\"dateStart\":\"$dateStart\",\"dateEnd\":\"$dateEnd\",\"media\":\"EMAIL\",\"properties\":\"total\"}");
curl_setopt($ch, CURLOPT_HTTPHEADER, array("Accept: application/json"));
$response = curl_exec($ch);
curl_close($ch);

if (false === $response) {
  // gérer l'erreur
} else {
  $result = json_decode($response);
  $total = $result->total;
  $command = 'sudo /usr/local/bin/newspaper -m "EMAIL: ' . $total . '"';
  exec($command, $result);
}
?>
