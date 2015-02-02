<?php

/**
 * @file
 * Administration callbacks for the learning_registry module
 */


/**
 * Form constructor to for basic search form for Learning Registry.
 */
function learning_registry_content_form($form, &$form_state) {

  if (variable_get('learning_registry_node_url', NULL) == NULL) {
    drupal_set_message(t('A Learning Registry URL has not yet been configured in the administration settings.
        Please visit Site Configuration -> Learning Registry settings to configure a URL before continuing.'), 'error');
  }
  else {
  $output = '';

  $search = '';
    $search = variable_get('learning_registry_node_url', NULL) . '/slice?';
    $result = drupal_http_request($search);
    $result_array = drupal_json_decode($result->data);
    $output = call_user_func('_learning_registry_dublincore_slice', $result_array);

    $form_state['storage']['results'] = $output;
	}	
    
      $form['learning_registry']['results'] = array(
        '#markup' => $form_state['storage']['results'],
      );

  return $form;
}


/**
 * Slice callback to display results.
 *
 * @param array $result_array
 *   Raw array of results from slice method.
 *
 * @return string
 *   HTML reprsentation of data.
 */
function _learning_registry_dublincore_slice($result_array) {
  $header = array();
  $header[] = 'Node ID';
  $header[] = 'Title'; 
  $header[] = 'LR Doc Id';
  $header[] = 'Date published';
  $header[] = 'Date updated';
  $header[] = 'Date deleted';
  $header[] = 'Queued';

  $rows = array();
  $docs = $result_array['documents'];
  foreach ($docs as $row) {
    if ($row['resource_data_description']['doc_type'] == 'resource_data') {
      $payload_schema = (isset($row['resource_data_description']['payload_schema'])) ? $row['resource_data_description']['payload_schema'] : "";
      $url = (isset($row['resource_data_description']['resource_locator'])) ? $row['resource_data_description']['resource_locator'] : "";
      $create_date = ($row['resource_data_description']['create_timestamp']) ? $row['resource_data_description']['create_timestamp'] : "";
      $create_date = drupal_substr($create_date, 0, 10);
	  $update_date = ($row['resource_data_description']['update_timestamp']) ? $row['resource_data_description']['update_timestamp'] : "";
      $update_date = drupal_substr($update_date, 0, 10);
	  $delete_date = ($row['resource_data_description']['payload_placement'] == "none") ? $update_date : "";
	  $update_date = ($delete_date != "") ? "" : $update_date;
      $delete_date = drupal_substr($delete_date, 0, 10);
	  $doc_id = $row['resource_data_description']['doc_ID'];
	  $q = db_select('learning_registry','l');
		$q->condition('l.docid',$doc_id,'=');
		$q->fields('l',array('nid','queued'));
		$result = $q->execute();
		$record = $result->fetchAssoc();
	  $nid = $record['nid'];
	  $queued = ($record['queued'] == 1) ? "True" : "False";
      $title = (isset($row['resource_data_description']['resource_data']['name'])) ? $row['resource_data_description']['resource_data']['name'] : "";
	  

      if (gettype($payload_schema) == 'array') {
          if (drupal_strlen($title) == 0) {
            $title = "[not provided]";
          }
          $title = check_plain($title);
          $link = l($title, $url, array('attributes' => array('target' => '_blank')));
          $create_date = check_plain($create_date);
          $rows[] = array($nid, $link, $doc_id, $create_date, $update_date, $delete_date, $queued);
       }
      }
    }
  
  return theme('table', array('header' => $header, 'rows' => $rows));
}
