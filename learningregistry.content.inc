<?php

/**
 * @file
 * Administration callbacks for the learningregistry module
 */
// much of this is lifted from the "Creating Drupal 6.x modules" docs on the
// Drupal website http://drupal.org/node/231276 and the "Writing a Module"
// chapter of Pro Drupal Development 2nd Edition by John K. VanDyk.  ISBN-13:
// 978-1430209898

// Thanks to Lullabot for the example on same page search results!
// http://www.lullabot.com/articles/drupal-5-making-forms-that-display-their-own-results

/**
 * Form builder. Basic search form for Learning Registry.
 *
 * @ingroup forms
 */
function learningregistry_content_form($form_values = NULL) {

  $form['#redirect'] = FALSE;

    $form['learningregistry'] = array(
        '#type' => 'fieldset',
        '#title' => t('Search'),
    );

    $form['learningregistry']['search_identity'] = array(
        '#type' => 'textfield',
        '#title' => t('Identity'),
        '#size' => 128,
        '#maxlength' => 1024,
        '#description' => t('The name of a person or organization that is the submitter, author, owner, or curator.'),
    );

    $form['learningregistry']['search_tags'] = array(
        '#type' => 'textfield',
        '#title' => t('Tags'),
        '#size' => 128,
        '#maxlength' => 1024,
        '#description' => t('The URL of the Learning Registry node.'),
    );

    $form['learningregistry']['search_from'] = array(
        '#type' => 'textfield',
        '#title' => t('From date'),
        '#size' => 24,
        '#maxlength' => 128,
        '#description' => t('The URL of the Learning Registry node.'),
    );

    $form['learningregistry']['search_until'] = array(
        '#type' => 'textfield',
        '#title' => t('Until date'),
        '#size' => 24,
        '#maxlength' => 128,
        '#description' => t('The URL of the Learning Registry node.'),
    );

    $form['learningregistry']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Search'),
    );

    // return system_settings_form($form);
  
    if ($form_values != NULL)  {
    // $form_values is populated, which means we're coming in a second
    // time. Let's display the results instead of the original form fields.
    $form['results'] = array(
        '#type' => 'item',
       /*  '#title' => t('The results of your form submission'), */
        '#value' => _learningregistry_content_form_values($form_values),
    );
  }

  return $form;
}

function _learningregistry_content_form_values($form_values = array()) {

  
  $identity = $form_state['values']['search_identity'];
  $tags = $form_state['values']['search_tags'];
  $from = $form_state['values']['search_from'];
  $until = $form_state['values']['search_until'];

  $search = '';

  //TODO:  need to beef this up, not a true search - one or the other, need to add & support
  if (strlen($identity) > 0) {
    $search = 'identity=' . $identity;
  }
  if (strlen($tags) > 0) {
    $search = 'tags=' . $tags;
  }

  //dsm($form_state);

  $ch = curl_init();
  $curl_options = array(
      CURLOPT_URL => variable_get('learningregistry_node_url', NULL) . "/slice?" . $search,
      CURLOPT_RETURNTRANSFER => 1,
  );
  // file_put_contents("/tmp/foo.json", $json);
  curl_setopt_array($ch, $curl_options);
  // $result=curl_exec($ch);
  // $error = curl_error($ch);
  curl_close($ch);

  //dsm($result);
  $result_array = json_decode($result);
  // dsm($result_array);
  //dsm($error);

  $header = array();
  $header[] = 'test';
  $header[] = 'test 2';
  $header[] = 'test 3';

  $rows = array(
      // Simple row
      array(
          'Cell 1', 'Cell 2', 'Cell 3'
      ),
      // Row with attributes on the row and some of its cells.
      array(
          'data' => array('Cell 1', array('data' => 'Cell 2', 'colspan' => 2)), 'class' => 'funky'
      )
  );


  $output = theme_table($header, $rows);
  
  // echo $output;
  // return $output;
  //theme('table', $headers, $result_array);
  //drupal_render($output);
  //echo $output;
  // I'd like to output results here!!

  
  
  
  
  
  
  $header = array(t('Key'), t('Value'));
  $rows = array();
  foreach ($form_values['post'] as $key => $value) {
    $row = array();
    $row[] = $key;
    $row[] = $value;
    $rows[] = $row;
  }

  dsm($form_values);
  
  return theme('table', $header, $rows);
}

// from - YYYY-MM-DD
// until - YYYY-MM-DD
// identity
// any_tags

// ids_only (default, false - optional)
// resumption_token (optional - to resume a search)