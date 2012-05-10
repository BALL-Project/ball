// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/DOCKING/GENETICDOCK/parameter.h>

namespace BALL
{
  GenericParameter::GenericParameter()
  {
  }

  GenericParameter::GenericParameter(const GenericParameter& p)
    :name_(p.name_)
  {
  }

  GenericParameter::~GenericParameter()
  {
  }

  const GenericParameter& GenericParameter::operator = (const GenericParameter& p)
  {
    if (this == &p)
      return *this;

    name_ = p.name_;

    return *this;
  }

  String GenericParameter::getName()
  {
    return name_;
  }

  DoubleParameter::DoubleParameter()
  {
    name_ = "DoubleParameter";
  }

  DoubleParameter::DoubleParameter(const DoubleParameter& p)
    :GenericParameter(p)
  {
  }

  DoubleParameter::~DoubleParameter()
  {
  }

  const DoubleParameter& DoubleParameter::operator = (const DoubleParameter& p)
  {
    if (this == &p)
      return *this;

    GenericParameter::operator = (p);

    values = p.values;

    return *this;
  }


  QuaternionParameter::QuaternionParameter()
  {
    name_ = "QuaternionParameter";
  }

  QuaternionParameter::QuaternionParameter(const QuaternionParameter& p)
    :GenericParameter(p)
  {
  }

  QuaternionParameter::~QuaternionParameter()
  {
  }

  const QuaternionParameter& QuaternionParameter::operator = (const QuaternionParameter& p)
  {
    if (this == &p)
      return *this;

    GenericParameter::operator = (p);

    quat = p.quat;

    return *this;
  }

}
