<?php

/**
 * @file
 * Administration callbacks for the learningregistry module
 */

// much of this is lifted from the "Creating Drupal 6.x modules" docs on the
// Drupal website http://drupal.org/node/231276 and the "Writing a Module"
// chapter of Pro Drupal Development 2nd Edition by John K. VanDyk.  ISBN-13:
// 978-1430209898

/**
 * Form builder. Basic search form for Learning Registry.
 *
 * @ingroup forms
 */
function learningregistry_content_form() {
  
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
    return $form;
}

function learningregistry_content_form_submit($form, $form_state) {
  
  $identity = $form_state['values']['search_identity'];
  $tags = $form_state['values']['search_tags'];
  $from = $form_state['values']['search_from'];
  $until = $form_state['values']['search_until'];
  
  $search = '';
  
  //TODO:  need to beef this up, not a true search - one or the other, need to add & support
  if (strlen($identity) > 0) { $search='identity='.$identity; }
  if (strlen($tags) > 0) { $search='tags='.$tags; }
  
  //dsm($form_state);
  
  $ch = curl_init();
  $curl_options = array(
    CURLOPT_URL => variable_get('learningregistry_node_url', NULL) . "/slice?" . $search,
    CURLOPT_RETURNTRANSFER => 1,
  );
  // file_put_contents("/tmp/foo.json", $json);
  curl_setopt_array($ch, $curl_options);
  $result=curl_exec($ch);
  $error = curl_error($ch);
  curl_close($ch);
  
  dsm($result);
  $result_array = json_decode($result);
  // dsm($result_array);
  
  dsm($error);
  
}


// from - YYYY-MM-DD
// until - YYYY-MM-DD
// identity
// any_tags

// ids_only (default, false - optional)
// resumption_token (optional - to resume a search)