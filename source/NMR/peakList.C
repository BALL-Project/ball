// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: peakList.C,v 1.4 2003/04/22 14:34:07 sneumann Exp $

#include <BALL/NMR/peakList.h>

namespace BALL
{

	template <>
	PeakList<Peak2D>::Position PeakList<Peak2D>::getMinPosition() const
	{
		ConstIterator it = PeakList<Peak2D>::begin();
		Peak2D::Position min(Limits<float>::max(), Limits<float>::max());
		for (; it != PeakList<Peak2D>::end(); ++it)
		{
			min.x = std::min(min.x, it->getPosition().x);
			min.y = std::min(min.y, it->getPosition().y);
		}

		return min;
	}

	template <>
	PeakList<Peak2D>::Position PeakList<Peak2D>::getMaxPosition() const
	{
		ConstIterator it = PeakList<Peak2D>::begin();
		Peak2D::Position max(-Limits<float>::max(), -Limits<float>::max());
		for (; it != PeakList<Peak2D>::end(); ++it)
		{
			max.x = std::max(max.x, it->getPosition().x);
			max.y = std::max(max.y, it->getPosition().y);
		}

		return max;
	}

	template <>
	PeakList<Peak3D>::Position PeakList<Peak3D>::getMinPosition() const
	{
		ConstIterator it = PeakList<Peak3D>::begin();
		Peak3D::Position min(Limits<float>::max(), Limits<float>::max(), Limits<float>::max());
		for (; it != PeakList<Peak3D>::end(); ++it)
		{
			min.x = std::min(min.x, it->getPosition().x);
			min.y = std::min(min.y, it->getPosition().y);
			min.z = std::min(min.z, it->getPosition().z);
		}

		return min;
	}

	template <>
	PeakList<Peak3D>::Position PeakList<Peak3D>::getMaxPosition() const
	{
		ConstIterator it = PeakList<Peak3D>::begin();
		Peak3D::Position max(-Limits<float>::max(), -Limits<float>::max(), -Limits<float>::max());
		for (; it != PeakList<Peak3D>::end(); ++it)
		{
			max.x = std::max(max.x, it->getPosition().x);
			max.y = std::max(max.y, it->getPosition().y);
			max.z = std::max(max.z, it->getPosition().z);
		}

		return max;
	}

} // namespace BALL







