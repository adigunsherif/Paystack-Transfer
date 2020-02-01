<?php

function custom_transfer_rules_event_info() {
  $items = array(
    'on_fund_transfer' => array(
      'label' => t('Fund is being transfered'),
      'group' => t('Custom'),
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
        'amount' => array(
          'type' => 'text',
          'label' => t('Amount'),
        ),
      ),
    ),

  );

  return $items;
}

/**
 * Implement hook_rules_action_info().
 */

function custom_transfer_rules_action_info() {

	$actions = array(
		'custom_transfer_register_on_paystack' => array(
		  'label'=> t('Register on Paystack'),
		  'group' => t('Custom'),
		  'parameter' => array(
				'name' => array(
					'type' => 'text',
					'label' => t('Recipient Name'),
        ),
        'account_number' => array(
					'type' => 'text',
          'label' => t('Account Number'),
          'description' => t('The 10 digits nuban number.'),
        ),
        'bank_code' => array(
					'type' => 'text',
          'label' => t('Bank Code'),
          'description' => t('See the list of available bank codes here https://developers.paystack.co/reference#list-banks'),
        ),
      ),
      'provides' => array(
        'recipient_code' => array(
          'label' => t('Recipient Code'),
          'type' => 'text',
          'save' => TRUE,
        ),
      ),
    ),
    'custom_transfer_transfer_money' => array(
		  'label'=> t('Transfer fund to user'),
		  'group' => t('Custom'),
		  'parameter' => array(
				'amount' => array(
					'type' => 'text',
					'label' => t('Amount'),
        ),
        'recipient' => array(
					'type' => 'text',
          'label' => t('Recipient'),
          'description' => t('The recipient of the fund. This should be the recipient code from paystack'),
        ),
      ),
      'provides' => array(
        'response' => array(
          'label' => t('Response'),
          'type' => 'text',
        ),
      ),
    ),
	);


	return $actions;

}

// The callback for action
function custom_transfer_register_on_paystack($name, $account_number, $bank_code){

  $key = custom_transfer_secret_key();

  //make a call to the transfer API
  $url = 'https://api.paystack.co/transferrecipient';
  $body = array(
    "type" => "nuban",
    "name" => $name,
    "account_number" => $account_number,
    "bank_code" => $bank_code,
  );

  $options = array(
    'method' => 'POST',
    'headers' => array(
			'Authorization' => 'Bearer ' . $key,
			'Content-Type' => 'application/json',

		),
    'data' =>   json_encode($body),
  );

  $request = drupal_http_request($url, $options);
  if($request){
    $result = json_decode($request->data, true);
    if($result){
      if($result['status'] == TRUE){
        $code = $result['data']['recipient_code'];
        $message = "'{$name}' successfully added to paystack with returned code of {$code}";
        watchdog('custom_transfer', $message, array(), WATCHDOG_NOTICE);
        return array('recipient_code'=> $result['data']['recipient_code']);
      }else{
        $message = "attempt to add '{$name}' to paystack failed. {$result['message']}";
        watchdog('custom_transfer', $message, array(), WATCHDOG_ERROR);
      }
    }else{
      $message = "attempt to add '{$name}' to paystack failed. Couldn't decode response";
      watchdog('custom_transfer', $message, array(), WATCHDOG_ERROR);
    }
  }else{
    $message = "attempt to add '{$name}' to paystack failed. No connection";
      watchdog('custom_transfer', $message, array(), WATCHDOG_ERROR);
  }



}


/**
 * Function to initiate paystack transfer
 */
function custom_transfer_transfer_money($amount, $recipient){
  $key = custom_transfer_secret_key();

  //make a call to the transfer API
  $url = 'https://api.paystack.co/transfer';
  $body = array(
    "amount" => $amount * 100,
    "recipient" => $recipient,
  );

  $options = array(
    'method' => 'POST',
    'headers' => array(
			'Authorization' => 'Bearer ' . $key,
			'Content-Type' => 'application/json',

		),
    'data' =>   json_encode($body),
  );

  $request = drupal_http_request($url, $options);
  if($request){
    $result = json_decode($request->data, true);
    if($result){
      if($result['status'] == TRUE){
        return array('response'=>$result['data']['status']);

      }
    }
  }

}