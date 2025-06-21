#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "msp.h"
#include "msp_datatypes.h"

namespace py = pybind11;
using namespace MSP;

PYBIND11_MAKE_OPAQUE(std::vector<uint8_t>);

PYBIND11_MODULE(pyMsp, m) {
    py::bind_vector<std::vector<uint8_t>>(m, "Payload");

    py::class_<msp>(m, "msp")
        .def(py::init<const std::string, int>())
        .def("sendCmd", &msp::sendCmd)
        .def("getData", &msp::getData)
        .def("sendMspCmd", py::overload_cast<uint8_t, const std::any&>(&msp::sendMspCmd))
        .def("sendMspCmd", py::overload_cast<uint8_t>(&msp::sendMspCmd))
        .def("processData", &msp::processData)
        .def("checkMspResponse", &msp::checkMspResponse)
        .def("getName", &msp::getName)
        .def("getVtx", &msp::getVtx)
        .def("setVtx", py::overload_cast<uint8_t, uint8_t>(&msp::setVtx))
        .def("setVtx", py::overload_cast<uint16_t>(&msp::setVtx))
        .def("getAttitude", &msp::getAttitude)
        .def("getRC", &msp::getRC)
        .def("getRawIMU", &msp::getRawIMU);

    py::class_<vtxConfigIn>(m, "VTXConfig")
        .def(py::init<const std::vector<uint8_t>&>())  // enable construction from Python
        .def_readwrite("vtxType", &vtxConfigIn::vtxType)
        .def_readwrite("vtxBand", &vtxConfigIn::vtxBand)
        .def_readwrite("vtxChannel", &vtxConfigIn::vtxChannel)
        .def_readwrite("vtxPower", &vtxConfigIn::vtxPower)
        .def_readwrite("vtxStatus", &vtxConfigIn::vtxStatus)
        .def_readwrite("vtxFreq", &vtxConfigIn::vtxFreq)
        .def_readwrite("vtxDeviceReady", &vtxConfigIn::vtxDeviceReady)
        .def_readwrite("vtxLowPowerDisarm", &vtxConfigIn::vtxLowPowerDisarm)
        .def_readwrite("vtxPitFreq", &vtxConfigIn::vtxPitFreq)
        .def_readwrite("vtxTableAvail", &vtxConfigIn::vtxTableAvail)
        .def_readwrite("vtxTableBands", &vtxConfigIn::vtxTableBands)
        .def_readwrite("vtxTableChannels", &vtxConfigIn::vtxTableChannels)
        .def_readwrite("vtxPowerLevels", &vtxConfigIn::vtxPowerLevels)
        .def_readwrite("padding1", &vtxConfigIn::padding1)
        .def_readwrite("padding2", &vtxConfigIn::padding2)
        .def_readwrite("padding3", &vtxConfigIn::padding3)
        .def_readwrite("padding4", &vtxConfigIn::padding4);

    py::class_<rcChannelData>(m, "RCChannelData")
        .def(py::init<const std::vector<uint16_t>&>())  // default constructor
        .def_readwrite("roll", &rcChannelData::roll)
        .def_readwrite("pitch", &rcChannelData::pitch)
        .def_readwrite("yaw", &rcChannelData::yaw)
        .def_readwrite("throttle", &rcChannelData::throttle)
        .def_readwrite("aux4", &rcChannelData::aux4);
    
    py::class_<imuData>(m, "IMUData")
        .def(py::init<const std::vector<float>&>())  // default constructor
        .def_readwrite("accel_x", &imuData::accel_x)
        .def_readwrite("accel_y", &imuData::accel_y)
        .def_readwrite("accel_z", &imuData::accel_z)
        .def_readwrite("gyro_x", &imuData::gyro_x)
        .def_readwrite("gyro_y", &imuData::gyro_y)
        .def_readwrite("gyro_z", &imuData::gyro_z)
        .def_readwrite("mag_x", &imuData::mag_x)
        .def_readwrite("mag_y", &imuData::mag_y)
        .def_readwrite("mag_z", &imuData::mag_z);
}

