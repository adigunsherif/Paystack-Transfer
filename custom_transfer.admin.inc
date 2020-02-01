<?php

function custom_transfer_admin() {
  $form = array();

  $form['custom_transfer_webform_field'] = array(
    '#type' => 'textfield',
    '#title' => t('Webform Field name'),
    '#default_value' => variable_get('custom_transfer_webform_field', 'field_webform'),
    '#description' => t('The field name of the attached webform'),
    '#required' => TRUE,
  );

  $form['custom_transfer_merchant_charges'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant Charges'),
    '#default_value' => variable_get('custom_transfer_merchant_charges', '2.5'),
    '#description' => t('The percentage to charge each merchant'),
    '#element_validate' => array('element_validate_number'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
