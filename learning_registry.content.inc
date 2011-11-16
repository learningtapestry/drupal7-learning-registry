<?php

/**
 * @file
 * Administration callbacks for the learning_registry module
 */
// much of this is lifted from the "Creating Drupal 6.x modules" docs on the
// Drupal website http://drupal.org/node/231276 and the "Writing a Module"
// chapter of Pro Drupal Development 2nd Edition by John K. VanDyk.  ISBN-13:
// 978-1430209898
//
// Thanks to Lullabot for the example on same page search results!
// http://www.lullabot.com/articles/drupal-5-making-forms-that-display-their-own-results

/**
 * Form builder. Basic search form for Learning Registry.
 *
 * @ingroup forms
 */
function learning_registry_content_form($form_values = NULL) {

  $form['#redirect'] = FALSE;

  $form['learning_registry'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search'),
  );

  $form['learning_registry']['search_identity'] = array(
      '#type' => 'textfield',
      '#title' => t('Identity'),
      '#size' => 72,
      '#maxlength' => 1024,
      '#description' => t('The name of a person or organization that is the submitter, author, owner, or curator.'),
  );

  $form['learning_registry']['search_tags'] = array(
      '#type' => 'textfield',
      '#title' => t('Tags'),
      '#size' => 72,
      '#maxlength' => 1024,
      '#description' => t('List of tags, matched against keys, payload_schema, or resource_data_type.'),
  );

  $form['learning_registry']['search_from'] = array(
      '#type' => 'textfield',
      '#title' => t('From date'),
      '#size' => 24,
      '#maxlength' => 128,
      '#description' => t('Date of publication to learning registry. Format:  YYYY-MM-DD.'),
  );

  $form['learning_registry']['search_until'] = array(
      '#type' => 'textfield',
      '#title' => t('Until date'),
      '#size' => 24,
      '#maxlength' => 128,
      '#description' => t('Date of publication to learning registry. Format:  YYYY-MM-DD.'),
  );

  $form['learning_registry']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
  );

  // return system_settings_form($form);

  if ($form_values != NULL) {
    // $form_values is populated, which means we're coming in a second
    // time. Let's display the results instead of the original form fields.
    $form['results'] = array(
        '#type' => 'item',
        /*  '#title' => t('The results of your form submission'), */
        '#value' => _learning_registry_content_form_values($form_values),
    );
  }

  return $form;
}

function _learning_registry_content_form_values($form_values = array()) {

  $identity = $form_values['post']['search_identity'];
  $tags = $form_values['post']['search_tags'];
  $from = $form_values['post']['search_from'];
  $until = $form_values['post']['search_until'];

  $search = '';

  if (drupal_strlen($identity) > 0) {
    $search = '&identity=' . $identity;
  }
  if (drupal_strlen($tags) > 0) {
    $search = '&any_tags=' . $tags;
  }
  if (drupal_strlen($from) > 0) {
    $search = '&from=' . $from;
  }
  if (drupal_strlen($until) > 0) {
    $search = '&until=' . $until;
  }

  if ($search != '') {

    $search = variable_get('learning_registry_node_url', NULL) . "/slice?" . $search;

    // dsm($search);

    $ch = curl_init();
    $curl_options = array(
        CURLOPT_URL => $search,
        CURLOPT_RETURNTRANSFER => 1,
    );

    curl_setopt_array($ch, $curl_options);
    $result = curl_exec($ch);
    $error = curl_error($ch);
    curl_close($ch);

    // dsm($result);
    $result_array = json_decode($result);
    // dsm($result_array);
    // dsm($error);

    $header = array();
    $header[] = 'Resource';
    $header[] = 'Submitter';
    $header[] = 'Description';
    $header[] = 'Create Date';

    $rows = array();
    $docs = $result_array->documents;
    // dsm($docs);

    foreach ($docs as $row) {
      $url = $row->resource_data_description->resource_locator;
      $payload = $row->resource_data_description->resource_data;
      $submitter = $row->resource_data_description->identity->submitter;
      $create_date = $row->resource_data_description->create_timestamp;
      $create_date = drupal_substr($create_date, 0, 10);


      $title = "";

      if (gettype($payload) == "string") {
        $xml = simplexml_load_string($payload);
        $dc = $xml->children("http://purl.org/dc/elements/1.1/");
        $title = $dc->title;
        $desc = $dc->description;
      }

      if (drupal_strlen($title) < 1) {
        $title = "[unknown]";
      }

      $link = "<a target='_blank' href='" . $url . "'>" . $title . "</a>";

      $rows[] = array($link, $submitter, $desc, $create_date);
    }

    $output = theme_table($header, $rows);

    return $output;
  }
}

// from - YYYY-MM-DD
// until - YYYY-MM-DD
// identity
// any_tags

// ids_only (default, false - optional)
// resumption_token (optional - to resume a search)