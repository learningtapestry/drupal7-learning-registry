<?php

/**
 * @file
 * Administration callbacks for the learning_registry module
 */

// much of this is lifted from the "Creating Drupal 6.x modules" docs on the
// Drupal website http://drupal.org/node/231276 and the "Writing a Module"
// chapter of Pro Drupal Development 2nd Edition by John K. VanDyk.  ISBN-13:
// 978-1430209898

/**
 * Form builder. Configure learning_registry module.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function learning_registry_admin_settings() {
  // Get node types with internal names as keys and "friendly" names as
  // values.
  // $options = node_get_types('names');
  $options = node_get_types();
  $type_names = array();
  $cck_fields = array();

  foreach ($options as $option) {
      $type_names[$option->type] = $option->name;
      $type = content_types($option->type);
      $cck_fields[$option->type] = $type['fields'];
  }

  $form = array();

  $form['learning_registry_node_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Learning Registry node URL'),
    '#size' => 128,
    '#maxlength' => 1024,
    '#default_value' => variable_get('learning_registry_node_url', NULL),
    '#description' => t('The URL of the Learning Registry node.'),
    '#required' => TRUE,
  );

  // TODO: figure out if we need to validate this information.
  $form['learning_registry_submitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Submitter'),
    '#size' => 128,
    '#maxlength' => 1024,
    '#default_value' => variable_get('learning_registry_submitter', NULL),
    '#description' => t('Identifies the submitter of a Learning Registry resource document.'),
    '#required' => TRUE,
  );

  $form['learning_registry_tos'] = array(
    '#type' => 'textfield',
    '#title' => t('Terms of Service URL'),
    '#size' => 128,
    '#maxlength' => 1024,
    '#default_value' => variable_get('learning_registry_tos', NULL),
    '#description' => t('Specifies the terms of service URL for the items submitted to the Learning Registry.'),
    '#required' => TRUE,
  );

  $form['learning_registry_queuing'] = array(
    '#type' => 'radios',
    '#title' => t('Immediate or Queued Publishing'),
    '#default_value' => variable_get('learning_registry_queuing', 0),
    '#description' => t('Selects if submissions to the Learning Registry will publish immediately or queued in a batch to publish when cron executes.'),
    '#options' => array(t('Immediate'), t('Queued')),
    '#required' => TRUE,
  );

  $form['learning_registry_url_type'] = array(
    '#type' => 'radios',
    '#title' => t('Publish URL Type'),
    '#default_value' => variable_get('learning_registry_url_type', 0),
    '#description' => t('Determines the URL to the published Drupal node.  Node ID is the internal Drupal node (node/1234).  Node alias is the alias provided for the node (blog/this-is-a-test).'),
    '#options' => array(t('Node ID'), t('Node alias')),
    '#required' => TRUE,
  );

  $form['learning_registry_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Publish metadata for these content types'),
    '#options' => $type_names,
    '#default_value' => variable_get('learning_registry_node_types', array()),
    '#description' => t('Metadata for these node types will be published to the specified Learning Registry node.'),
  );

  foreach ($type_names as $type_key => $type_name) {
    if (count($cck_fields[$type_key])) {

      $cck_checkboxes = array();

      foreach ($cck_fields[$type_key] as $cck_field) {
          $field_name = $cck_field['field_name'];
          $cck_checkboxes[$cck_field['field_name']] = $cck_field['widget']['label'];
      }

      $form['learning_registry_node_type_fieldset_' . $type_key] = array(
        '#type' => 'fieldset',
        '#title' => t($type_name),
        '#description' => t('Metadata for these CCK fields will be published to the specified Learning Registry node.'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $form['learning_registry_node_type_fieldset_' . $type_key]['learning_registry_node_type_option_' . $type_key] = array(
        '#type' => 'checkboxes',
        '#title' => t('Publish these CCK fields for ' . $type_name),
        '#options' => $cck_checkboxes,
        '#default_value' => variable_get('learning_registry_node_type_option_' . $type_key, array()),
      );
    }
  }

  return system_settings_form($form);
}


/**
 * Validate the learning_registry configuration form.
 *
 * @param array $form
 *   The array that describes the configuration form.
 * @param array $form_state
 *   The values of the array.
 */
function learning_registry_admin_settings_validate($form, $form_state) {
  $lr_node_url = $form_state['values']['learning_registry_node_url'];
  if (!valid_url($lr_node_url, TRUE)) {
    form_set_error('learning_registry_node_url', t('Please provide a valid URL.'));
  }
}

