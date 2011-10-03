<?php

/**
 * @file
 * Stripe administration and module settings UI.
 */

/**
 * Menu callback: configure Stripe API Keys
 */
function stripe_admin_keys() {
  $form['API_Keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Keys'),
    '#collapsible' => FALSE,
  );

  $fields = array(
    'publishable_test',
    'publishable_live',
    'secret_test',
    'secret_live',
  );

  $active_key = variable_get('stripe_active_key', '');
  foreach ($fields as $field) {
    $title = ucwords(str_replace('_', ' ', $field));
    $form['API_Keys']['stripe_' . $field] = array(
      '#type' => 'textfield',
      '#title' => t($title),
      '#size' => 35,
      '#default_value' => variable_get('stripe_' . $field, ''),
    );
    $form['API_Keys']['active_' . $field] = array(
      '#prefix' => '<div class="container-inline form-item">',
      '#suffix' => '</div>'
    );
    $form['API_Keys']['active_' . $field][$field] = array(
      '#type' => 'radio',
      '#title' => t('Activate'),
      '#return_value' => $field,
      '#default_value' => $active_key == $field,
      '#parents' => array('stripe_active_key'),
    );
    if ($active_key == $field) {
      $form['API_Keys']['active_' . $field][$field]['#attributes'] = array('class' => array('active'));
      $form['API_Keys']['active_' . $field][$field]['#title'] = t('Active');
    }
  }

  // Attach a little custom css.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'stripe') . '/stripe.css',
  );

  // Attach a little custom js.
  $form['#attached']['js'] = array(
    drupal_get_path('module', 'stripe') . '/stripe.js',
  );

  $form['#submit'][] = 'stripe_admin_config_submit';

  return system_settings_form($form);
}

/**
 * Process result from stripe config form.
 */
function stripe_admin_config_submit($form, &$form_state) {

}

function stripe_admin_test() {
/* From example on https://stripe.com/api

<form action="" method="POST" id="payment-form">
    <div class="form-row">
        <label>Card Number</label>
        <input type="text" size="20" autocomplete="off" class="card-number"/>
    </div>
    <div class="form-row">
        <label>CVC</label>
        <input type="text" size="4" autocomplete="off" class="card-cvc"/>
    </div>
    <div class="form-row">
        <label>Expiration (MM/YYYY)</label>
        <input type="text" size="2" class="card-expiry-month"/>
        <span> / </span>
        <input type="text" size="4" class="card-expiry-year"/>
    </div>
    <button type="submit" class="submit-button">Submit Payment</button>
</form>

*/
  $form['card_number'] = array(
    '#type' => 'textfield',
    '#title' => t('Card Number:'),
    '#size' => 20,
    '#default_value' => '4242424242424242',
    '#attributes' => array(
      'autocomplete' => 'off',
      'class' => array('card-number'),
    ),
  );

  $form['card_cvc'] = array(
    '#type' => 'textfield',
    '#title' => t('CVC:'),
    '#size' => 4,
    '#default_value' => '123',
    '#attributes' => array(
      'autocomplete' => 'off',
      'class' => array('card-cvc'),
    ),
  );

  $form['card_expiry_month'] = array(
    '#type' => 'textfield',
    '#title' => t('Expiration (MM/YYYY):'),
    '#size' => 2,
    '#default_value' => '10',
    '#attributes' => array(
      'class' => array('card-expiry-month'),
    ),
  );

  $form['card_expiry_year'] = array(
    '#type' => 'textfield',
    '#title' => t('/'),
    '#size' => 2,
    '#default_value' => '2020',
    '#attributes' => array(
      'class' => array('card-expiry-year'),
    ),
  );

  $form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount:'),
    '#prefix' => '<div class="container-inline form-item">',
    '#suffix' => '<em>cents</em></div>',
    '#default_value' => 99,
    '#size' => 6,
  );

  // Attach a little custom css.
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'stripe') . '/stripe.css',
  );

  // Retrieve the active API Key.
  $active_key_name = variable_get('stripe_active_key', '');
  $active_key = variable_get('stripe_' . $active_key_name, '');
  if ($active_key !== '') {
    // Add the submit button. Can't submit without a key.
    $form['card_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit Payment'),
      '#attributes' => array(
        'class' => array('submit-button'),
      ),
    );

    // Attach the javascript from Stripe.
    $form['#attached']['js'] = array(
      'https://js.stripe.com/v1/' => array(
        'type' => 'external',
      ),
      'jQuery(document).ready(function () { Stripe.setPublishableKey("' . $active_key . '"); });' => array(
        'type' => 'inline', 'scope' => 'footer', 'weight' => 5
      ),
    );
  }

  return $form;
}