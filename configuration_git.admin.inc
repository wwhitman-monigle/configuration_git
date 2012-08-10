<?php


/**
 * Administrative form
 *
 * @see configuration_git_menu()
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function configuration_git_config_form($form, &$form_state) {
  $form['configuration_git_repository'] = array(
    '#type' => 'textfield',
    '#title' => t('Platform git Respository'),
    '#default_value' => variable_get('configuration_git_repository', CONFIGURATION_GIT_RESPOSITORY),
    '#size' => 64,
    '#maxlength' => 254,
    '#description' => t('Enter the Git Repository Location Here (https://, Leave blank for local only.)'),
    '#required' => FALSE,
  );
  $form['configuration_git_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Platform git Name'),
    '#default_value' => variable_get('configuration_git_name', CONFIGURATION_GIT_NAME),
    '#size' => 64,
    '#maxlength' => 254,
    '#description' => t('Enter the Git Repository Default Name Address (.gitconfig)'),
    '#required' => TRUE,
  );
  $form['configuration_git_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Platform git Email'),
    '#default_value' => variable_get('configuration_git_email', CONFIGURATION_GIT_EMAIL),
    '#size' => 64,
    '#maxlength' => 254,
    '#description' => t('Enter the Git Repository Default Email Address (.gitconfig)'),
    '#required' => TRUE,
  );
  $form['configuration_git_remotename'] = array(
    '#type' => 'textfield',
    '#title' => t('Platform git Remote Name'),
    '#default_value' => variable_get('configuration_git_remotename', CONFIGURATION_GIT_REMOTENAME),
    '#size' => 64,
    '#maxlength' => 254,
    '#description' => t('Enter the Git Repository Default Email Address (.gitconfig)'),
    '#required' => TRUE,
  );
  $form['configuration_git_branch'] = array(
    '#type' => 'textfield',
    '#title' => t('Platform git Remote Branch'),
    '#default_value' => variable_get('configuration_git_branch', CONFIGURATION_GIT_BRANCH),
    '#size' => 32,
    '#maxlength' => 64,
    '#description' => t('Enter the Git Repository Default Branch (.gitconfig)'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Implements validation from the Form API.
 * 
 * @param $form
 *   A structured array containing the elements and properties of the form.
 * @param $form_state
 *   An array that stores information about the form's current state 
 *   during processing.
 */
function configuration_git_admin_config_system_form_validate($form, &$form_state){
  $form_variables = array('configuration_git_repository','configuration_git_name','configuration_git_email','configuration_git_remotename','configuration_git_branch');

  foreach($form_variables AS $var)
  {
    switch ($var) {
      case 'configuration_git_repository':
        if (($form_state['values'][$var])&&!(valid_url($form_state['values'][$var], TRUE))) {
          form_set_error($var, t('Put a valid Git Url Here (https://)'));
        }
        break;
      case 'configuration_git_email':
        if (!valid_email_address($configuration_git_email)){
          form_set_error('configuration_git_email', t('C\'mon man, your an admin right? Yu know what an email looks like?'));
        }
        break;
      default:
        if (!$form_state['values'][$var]) {
          form_set_error($form_state['values'][$var], t('Put anything here, make your mark. X will do.'));
        }
        break;
    }
    if (module_exists('configuration_variables')&&$form_state['values'][$var]!=  variable_get($var)) {
      configuration_check_configuration_variables($var);
    }
  }

  
  $status = configuration_git_status();
  if ($status['return_var'])
  {
	form_set_error('configuration_git_repository', t(implode('\n',$status['output'])));
  }
  
  $status = configuration_git_remote();
  if ($status['return_var'])
	  if (!stripos(implode(' ',$status['output']), CONFIGURATION_GIT_STATUS_REMOTEEXISTS))
      form_set_error('configuration_git_repository', t($status['return_var'].':'.implode('\n',$status['output'])));
    
}

/**
 * Administrative form
 *
 * @see configuration_git_menu()
 * @param type $form
 * @param type $form_state
 * @return type 
 */
function configuration_git_status_form($form, &$form_state) {
	
	configuration_git_status();
	$form['help_text'] = array(
	'#markup' => 'Below is the status from the current Git repository.',
	'#prefix' => '<div>',
	'#suffix' => '</div>',
	);
	
	$result = configuration_git_status();
	$form['status_text'] = array(
	'#markup' => str_ireplace('\n','<br />',implode('<br />',$result['output'])),
	'#prefix' => '<div>',
	'#suffix' => '</div>',
	);
	
	$form['status_text'] = array(
	'#markup' => str_ireplace('\n','<br />',implode('<br />',$result['output'])),
	'#prefix' => '<div>',
	'#suffix' => '</div>',
	);

	$form['buttons']['commit_changes'] = array(
	'#type' => 'submit',
	'#value' => t('Commit Changes'),
	'#weight' => 20,
	'#submit' => array('configuration_git_commit_all_submit'),
	);

	$form['buttons']['push_changes'] = array(
	'#type' => 'submit',
	'#value' => t('Push Changes'),
	'#weight' => 20,
	'#submit' => array('configuration_git_push_all_submit'),
	);

	return $form;
}

/**
 * Submit handler for commit clean up.
 */
function configuration_git_commit_all_submit($form, &$form_state) {

	$result = configuration_git_commit_all();
}

/**
 * Submit handler for commit clean up.
 */
function configuration_git_push_all_submit($form, &$form_state) {

	$result = configuration_git_push();
}