void loop() {

  switch (robotMode)
  {
  case isIdle:
      checkAlerts(); {
          if (mp27.getInterrupts(intFlags)){
            robotMode = isAlert;
          }
      break;

  case isActive:
      checkAlerts();
      break;

  case isAlert:
      break;
      
  case isError:
      break;

  default:
      break;

}
