 int get_dist()
  {
    return (dhigh - analogRead(AD_PIN))*dscale;
  }