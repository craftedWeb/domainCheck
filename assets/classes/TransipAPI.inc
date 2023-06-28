<?php
class TransipAPI {
	private string $endpointURL = "https://api.transip.nl/v6";
	private string $loginName = "YOUR LOGIN HERE";
	private string $privateKey = "-----BEGIN PRIVATE KEY-----YOUR KEY HERE-----END PRIVATE KEY-----";
	private string $accessToken = "";
	private bool $readOnly = true;
	private string $expirationTime = "30 minutes"; // Set the duration of the access token
 
  /**
	 * @var string
	*/
	private $signature;
	
	public string $postData;
	//public string $query;
	
	public string $responseHeader;
	public string $responseBody;
	
	public string $responseCode;
	
	public function __construct() {
	
	}
	
	public function createToken () {
		$requestBody = json_encode([
			'login'             => $this->loginName,
			'nonce'             => uniqid(),
			'read_only'         => $this->readOnly,
			'expiration_time'   => $this->expirationTime,
			'label'             => "",
			'global_key'        => false,
		]);
		
		// Create signature using the JSON encoded request body and your private key.
		$this->signature = $this->createSignature($requestBody);
		
		$responseJson = $this->requestToken($requestBody);
		
		#print_r($responseJson);
		
		if (!isset($responseJson->token)) {
			throw new \RuntimeException("An error occurred: {$responseJson->error}");
		}
		
		return $responseJson->token;
	}
	
	private function requestToken ($requestBody) {
		$url = $this->endpointURL . "/auth";
		
		$cURL = curl_init();
		
		curl_setopt_array($cURL, [
			CURLOPT_URL => $url,
			CURLOPT_POST => true,
			CURLOPT_POSTFIELDS => $requestBody,
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_HEADER => false,
			CURLINFO_HEADER_OUT => false,
			CURLOPT_HTTPHEADER => [
				"Content-Type: application/json",
				"Signature: " . $this->signature
			]
		]);
		
		$response = curl_exec($cURL);
		
		#print_r($response);
		
		/*if (curl_errno($cURL)) {
			throw new \Exception(curl_error($cURL));
		}*/
		
		curl_close($cURL);
		
		$responseJson = json_decode($response);
		return $responseJson;
	}
	
	private function createSignature ($parameters) {
		// Fixup our private key, copy-pasting the key might lead to whitespace faults
		if (!preg_match(
			'/-----BEGIN (RSA )?PRIVATE KEY-----(.*)-----END (RSA )?PRIVATE KEY-----/si',
			$this->privateKey,
			$matches
		)
		) {
			throw new \RuntimeException('Could not find a valid private key');
		}
		
		$key = $matches[2];
		$key = preg_replace('/\s*/s', '', $key);
		$key = chunk_split($key, 64, "\n");
		
		$key = "-----BEGIN PRIVATE KEY-----\n" . $key . "-----END PRIVATE KEY-----";
		
		if (!@openssl_sign($parameters, $signature, $key, OPENSSL_ALGO_SHA512)) {
			throw new \RuntimeException(
				'The provided private key is invalid'
			);
		}
		
		return base64_encode($signature);
	}
	
	public function checkDomain ($domainName) {
		$request = $this->sendRequest("domain-availability/{$domainName}/");
		
		$domain = $request->availability;
		
		$response = (object) [
			"domainName" => $domain->domainName,
			"status" => $domain->status,
			"actions" => $domain->actions
		];
		
		return $response;
	}
	
	public function checkMultipleDomains ($domainNames) {
		global $query;
		
		$query = "";
		
		array_walk($domainNames, function ($value) {
			global $query;
			
			$query .= (!empty($query) ? "&" : "") . "domainNames[]={$value}";
		});
		
		$request = $this->sendRequest("domain-availability/", "GET", $query);
		
		$response = [];
		
		foreach ($request->availability as $domain) {
			$response[] = (object) [
				"domainName" => $domain->domainName,
				"status" => $domain->status,
				"actions" => $domain->actions
			];
		}
		
		return $response;
	}
	
	public function getDomains () {
		$this->sendRequest("domains/");
		
		return $this->showBody();
	}
	
	public function getProducts () {
		$this->sendRequest("products/");
		
		return $this->showBody();
	}
	
	public function getTLDs () {
		$this->sendRequest("tlds/");
		
		return $this->showBody();
	}
	
	public function showRateLimitLimit () {
		$header = $this->responseHeader;
		
		preg_match("/X-Rate-Limit-Limit:\s?([0-9]+)/i", $header, $matches);
		
		return $matches[1];
	}
	
	public function showRateLimitRemaining () {
		$header = $this->responseHeader;
		
		preg_match("/X-Rate-Limit-Remaining:\s?([0-9]+)/i", $header, $matches);
		
		return $matches[1];
	}
	
	public function showResponseCode () {
		return $this->responseCode;
	}
	
	public function showHeader () {
		$response = $this->responseHeader;
		
		return $response;
	}
	
	public function showBody () {
		$response = $this->responseBody;
		
		$responseJSON = json_decode($response);
		
		return $responseJSON;
	}
	
	public function checkAPI () {
		$this->sendRequest("api-test/");
		
		$response = $this->responseBody;
		
		$responseJSON = json_decode($response);
		
		if ($responseJSON->ping == "pong")
			return true;
		else
			return false;
	}
	
	/**
	 * @throws Exception
	 */
	private function sendRequest ($endpoint, $method = "GET", $requestBody = "") {
		$url = $this->endpointURL . "/{$endpoint}";
		
		$cURL = curl_init();
		
		switch ($method){
			case "POST":
				curl_setopt($cURL, CURLOPT_POST, 1);
				
				if ($requestBody)
					curl_setopt($cURL, CURLOPT_POSTFIELDS, $requestBody);
				
				break;
			case "PUT":
				curl_setopt($cURL, CURLOPT_CUSTOMREQUEST, "PUT");
				
				if ($requestBody)
					curl_setopt($cURL, CURLOPT_POSTFIELDS, $requestBody);
				
				break;
			default:
				if ($requestBody)
					$url = sprintf("%s?%s", $url, $requestBody);
		}
		
		curl_setopt_array($cURL, [
			CURLOPT_URL => $url,
			CURLOPT_RETURNTRANSFER => true,
			CURLOPT_HEADER => true,
			CURLINFO_HEADER_OUT => true,
			CURLOPT_HTTPHEADER => [
				"Content-Type: application/json",
				"Authorization: Bearer " . $this->createToken()
			]
		]);
		
		$response = curl_exec($cURL);
		
		if (curl_errno($cURL)) {
			throw new \Exception(curl_error($cURL));
		}
		
		#$info = curl_getinfo($cURL, CURLINFO_HEADER_OUT);
		#echo "<pre>" . print_r($info, true) . "</pre>";
		
		$this->responseCode = curl_getinfo($cURL, CURLINFO_HTTP_CODE);
		
		curl_close($cURL);
		
		list($header, $body) = explode("\r\n\r\n", $response, 2);
		
		$this->responseHeader = $header;
		$this->responseBody = $body;
		
		$response = json_decode($body);
		
		if (isset($response->error)) {
			throw new \Exception($response->error);
		}
		elseif ($this->responseCode == "200") {
			$error = false;
		}
		elseif ($this->responseCode == "201") {
			$error = false;
		}
		elseif ($this->responseCode == "204") {
			$error = false;
		}
		elseif ($this->responseCode == "400") {
			$errorReason = "Bad Request";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "401") {
			$errorReason = "Unauthorized";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "403") {
			$errorReason = "Forbidden";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "404") {
			$errorReason = "Not Found";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "405") {
			$errorReason = "Method Not Allowed";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "406") {
			$errorReason = "Not Acceptable";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "408") {
			$errorReason = "Request Timeout";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "409") {
			$errorReason = "Conflict";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "422") {
			$errorReason = "Unprocessable Entity";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "429") {
			$errorReason = "Too Many Request";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "500") {
			$errorReason = "Internal Server Error";
			
			throw new \Exception($errorReason);
		}
		elseif ($this->responseCode == "501") {
			$errorReason = "Not Implemented";
			
			throw new \Exception($errorReason);
		}
		else {
			$errorReason = "Unknown";
			
			throw new \Exception($errorReason);
		}
		
		return $response;
	}
}

	
