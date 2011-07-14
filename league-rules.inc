<?php

function league_admin_leagues_rules() {
  if (!user_access("access administration pages")) {
    return message_access();  
  }
  
  $content = '<a href="?q=admin/league/rules/add">Add new rules</a><p/>';
  
  $result = db_query("SELECT * FROM {league_rules}");
  
  
  $content .= "<table>";
  $content .= "<tr>";
  $content .= '<th>' . t('Name'). '</th>';
  $content .= '<th>' . t('Main race') . '</th>';
  $content .= '<th>' .  t('Main race fastest lap') . '</th>';
  $content .= '<th>' . t('Sprint race') . '</th>';
  $content .= '<th>' . t('Sprint race fastest lap') . '</th>';
  $content .= '<th>' . t('Main race pole position') . '</th>';
  $content .= '<th>' . t('Sprint race pole position') . '</th>';
  $content .= '<th>&nbsp</th>';
  $content .= '</tr>';


   $i=0;
   while ($row = db_fetch_object($result)) {
  	 $content .= "<tr>";
     $content .= '<td>' . $row->name. '</td>';
     $content .= '<td>' .  _league_string_crop($row->main_race_points) . '</td>';
     $content .= '<td>' .  $row->main_race_fastest_lap . '</td>';
     $content .= '<td>' . _league_string_crop($row->sprint_race_points) . '</td>';
     $content .= '<td>' . $row->sprint_race_fastest_lap . '</td>';
     $content .= '<td>' .  $row->poleposition_points . '</td>';
     $content .= '<td>' .  $row->sprint_poleposition_points . '</td>';
     $content .= '<td><a href="?q=admin/league/rules/edit/' . $row->id . '">' . t("Edit") . '</a></td>';
     $content .= '</tr>';
  }
  $content .= "</table>";
  return $content;
}

function league_admin_leagues_rules_add($id = NULL) {
 return drupal_get_form('league_admin_leagues_rules_form', $id);   
}

function league_admin_leagues_rules_form($id = NULL) {

	if (isset($id)) {
		$values = league_admin_leagues_rules_values($id);

		if ($_POST['op'] == t('Delete')) {
      drupal_goto('admin/league/rules/delete/'. $id);
    }

	}

 $form = array();

  $form['name'] = array(
    '#type' => 'textfield', 
    '#title' => t('Name'),
    '#cols' => 32, 
    '#required' => TRUE,
    '#default_value' => $values['name']);

  $form['main_race_points'] = array(
    '#type' => 'textfield', 
    '#title' => t('Points main race'),
    '#cols' => 40, 
    '#required' => TRUE,
    '#default_value' => $values['main_race_points']);

  $form['main_race_fastest_lap'] = array(
    '#type' => 'textfield', 
    '#title' => t('Points main race fastest lap'),
    '#cols' => 2, 
    '#required' => TRUE,
    '#default_value' => $values['main_race_fastest_lap']);

  $form['sprint_race_points'] = array(
    '#type' => 'textfield', 
    '#title' => t('Points sprint race'),
    '#cols' => 40,
    '#default_value' => $values['sprint_race_points']);

  $form['sprint_race_fastest_lap'] = array(
    '#type' => 'textfield', 
    '#title' => t('Points sprint race fastest lap'),
    '#cols' => 2,
    '#default_value' => $values['sprint_race_fastest_lap']);      

  $form['poleposition_points'] = array(
    '#type' => 'textfield', 
    '#title' => t('Points for the pole position'),
    '#cols' => 2,
    '#default_value' => $values['poleposition_points']);

  $form['sprint_poleposition_points'] = array(
    '#type' => 'textfield', 
    '#title' => t('Points for the sprint race pole position'),
    '#cols' => 2,
    '#default_value' => $values['sprint_poleposition_points']);

  if (isset($id)) {
	   $form['delete'] = array('#type' => 'submit',
	      '#value' => t('Delete'),
	      '#weight' => 30,
	   );
	   $form['id'] = array('#type' => 'value', '#value' => $values['id']);
	}

  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Save'),
    '#default_value' => $values['name']);
	
	return $form;
}


function league_admin_leagues_rules_form_submit($form_id, $edit) {
  global $user;
  if (!user_access("access administration pages")) {
    return message_access();  
  }
  
  if ($edit['id'] > 0) {
    
    db_query("UPDATE {league_rules} SET main_race_points = '%s', main_race_fastest_lap = %d, " .  
      "sprint_race_points = '%s', sprint_race_fastest_lap = %d, name = '%s', poleposition_points = %d, sprint_poleposition_points = %d " .
      " WHERE id = '%d'", 
      $edit['main_race_points'], 
      $edit['main_race_fastest_lap'], 
      $edit['sprint_race_points'], 
      $edit['sprint_race_fastest_lap'], 
      $edit['name'], 
      $edit['poleposition_points'],
      $edit['sprint_poleposition_points'],
      $edit['id']); 
       
  } else {
    $result = db_query("INSERT INTO {league_rules} ". 
     "(id, main_race_points, main_race_fastest_lap, sprint_race_points, sprint_race_fastest_lap, name, poleposition_points, sprint_poleposition_points) " . 
     " VALUES('', '%s', %d, '%s', %d, '%s', %d, %d)", $edit['main_race_points'], $edit['main_race_fastest_lap'], $edit['sprint_race_points'], $edit['sprint_race_fastest_lap'], $edit['name'], $edit['poleposition_points'], $edit['sprint_poleposition_points']); 
 }
  
  return "admin/league/rules";
}

function league_admin_leagues_rules_delete($id) {  
	if (!isset($id)) {
    drupal_not_found();
    return;
  }

  $form = array();
  $form['id'] = array('#type' => 'value', '#value' => $id);

  return confirm_form($form,
    t('Are you sure you want to delete this league rules entry?'),
    $_GET['destination'] ? $_GET['destination'] : 'admin/league/rules',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
	
}

function league_admin_leagues_rules_delete_submit($form_id, $form_values) {
  
  if (!user_access("access administration pages")) {
    return message_access();  
  }
  
  db_query("DELETE FROM {league_rules} WHERE id = %d", $form_values['id']);

  return 'admin/league/rules';
}

function league_admin_leagues_rules_values($id) {
  

  $result = db_query("SELECT * FROM {league_rules} WHERE id=%d", $id);
    
  $values = array();

  if ($row = db_fetch_object($result)) {
    $values['id'] = $row->id;
    $values['name'] = $row->name;
    $values['main_race_points'] = $row->main_race_points;
    $values['main_race_fastest_lap'] = $row->main_race_fastest_lap;
    $values['sprint_race_points'] = $row->sprint_race_points;
    $values['sprint_race_fastest_lap'] = $row->sprint_race_fastest_lap;
    $values['poleposition_points'] = $row->poleposition_points;
    $values['sprint_poleposition_points'] = $row->sprint_poleposition_points;
  }

  return $values;
}

?>