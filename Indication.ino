void indicateProjects(String* projectsStatuses) {  
  for (int i = 0; i < PROJECTS_QUANTITY; i++) {
    _indicateProject(i, projectsStatuses[i]);
  }
}

void _indicateProject(int projectIndex, String value) {
  if (value == LEDMATRIX_COMMAND_RED) {
    digitalWrite(PROJECT_PINS[projectIndex], HIGH);
  }
}
