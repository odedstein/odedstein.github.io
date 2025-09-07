void JetColorMap(double x, double color[3])
{
  double a; // alpha

  if (x < 0)
  {
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    return;
  }
  else if (x < 0.125) 
  {
    a = x / 0.125;
    color[0] = 0;
    color[1] = 0;
    color[2] = 0.5 + 0.5 * a;
    return;
  }
  else if (x < 0.375)
  {
    a = (x - 0.125) / 0.25;
    color[0] = 0;
    color[1] = a;
    color[2] = 1;
    return;
  }
  else if (x < 0.625)
  {
    a = (x - 0.375) / 0.25;
    color[0] = a;
    color[1] = 1;
    color[2] = 1 - a;
    return;
  }
  else if (x < 0.875)
  {
    a = (x - 0.625) / 0.25;
    color[0] = 1;
    color[1] = 1 - a;
    color[2] = 0;
    return;
  }
  else if (x <= 1.0)
  {
    a = (x - 0.875) / 0.125;
    color[0] = 1 - 0.5 * a;
    color[1] = 0;
    color[2] = 0;
    return;
  }
  else
  {
    color[0] = 1;
    color[1] = 1;
    color[2] = 1;
    return;
  }
}

