// Copyright 2020 Dolphin Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include "Core/System.h"

#include <memory>

#include "AudioCommon/SoundStream.h"
#include "Core/Config/MainSettings.h"
#include "Core/CoreTiming.h"
#include "Core/HW/AudioInterface.h"
#include "Core/HW/CPU.h"
#include "Core/HW/DSP.h"
#include "Core/HW/DVD/DVDInterface.h"
#include "Core/HW/DVD/DVDThread.h"
#include "Core/HW/EXI/EXI.h"
#include "Core/HW/GPFifo.h"
#include "Core/HW/HSP/HSP.h"
#include "Core/HW/Memmap.h"
#include "Core/HW/MemoryInterface.h"
#include "Core/HW/ProcessorInterface.h"
#include "Core/HW/SI/SI.h"
#include "Core/HW/Sram.h"
#include "Core/HW/VideoInterface.h"
#include "Core/PowerPC/PowerPC.h"
#include "IOS/USB/Emulated/Skylander.h"
#include "VideoCommon/CommandProcessor.h"
#include "VideoCommon/Fifo.h"
#include "VideoCommon/GeometryShaderManager.h"
#include "VideoCommon/PixelEngine.h"
#include "VideoCommon/PixelShaderManager.h"
#include "VideoCommon/VertexShaderManager.h"

namespace Core
{
struct System::Impl
{
  explicit Impl(System& system)
      : m_audio_interface(system), m_core_timing(system), m_dsp(system), m_dvd_interface(system),
        m_gp_fifo(system), m_ppc_state(PowerPC::ppcState)
  {
  }

  std::unique_ptr<SoundStream> m_sound_stream;
  bool m_sound_stream_running = false;
  bool m_audio_dump_started = false;

  AudioInterface::AudioInterfaceManager m_audio_interface;
  CoreTiming::CoreTimingManager m_core_timing;
  CommandProcessor::CommandProcessorManager m_command_processor;
  CPU::CPUManager m_cpu;
  DSP::DSPManager m_dsp;
  DVDInterface::DVDInterfaceManager m_dvd_interface;
  DVDThread::DVDThreadState m_dvd_thread_state;
  ExpansionInterface::ExpansionInterfaceState m_expansion_interface_state;
  Fifo::FifoManager m_fifo;
  GeometryShaderManager m_geometry_shader_manager;
  GPFifo::GPFifoManager m_gp_fifo;
  HSP::HSPManager m_hsp;
  IOS::HLE::USB::SkylanderPortal m_skylander_portal;
  Memory::MemoryManager m_memory;
  MemoryInterface::MemoryInterfaceState m_memory_interface_state;
  PixelEngine::PixelEngineManager m_pixel_engine;
  PixelShaderManager m_pixel_shader_manager;
  PowerPC::PowerPCState& m_ppc_state;
  ProcessorInterface::ProcessorInterfaceManager m_processor_interface;
  SerialInterface::SerialInterfaceState m_serial_interface_state;
  Sram m_sram;
  VertexShaderManager m_vertex_shader_manager;
  VideoInterface::VideoInterfaceState m_video_interface_state;
};

System::System() : m_impl{std::make_unique<Impl>(*this)}
{
}

System::~System() = default;

void System::Initialize()
{
  m_separate_cpu_and_gpu_threads = Config::Get(Config::MAIN_CPU_THREAD);
  m_mmu_enabled = Config::Get(Config::MAIN_MMU);
  m_pause_on_panic_enabled = Config::Get(Config::MAIN_PAUSE_ON_PANIC);
}

SoundStream* System::GetSoundStream() const
{
  return m_impl->m_sound_stream.get();
}

void System::SetSoundStream(std::unique_ptr<SoundStream> sound_stream)
{
  m_impl->m_sound_stream = std::move(sound_stream);
}

bool System::IsSoundStreamRunning() const
{
  return m_impl->m_sound_stream_running;
}

void System::SetSoundStreamRunning(bool running)
{
  m_impl->m_sound_stream_running = running;
}

bool System::IsAudioDumpStarted() const
{
  return m_impl->m_audio_dump_started;
}

void System::SetAudioDumpStarted(bool started)
{
  m_impl->m_audio_dump_started = started;
}

AudioInterface::AudioInterfaceManager& System::GetAudioInterface() const
{
  return m_impl->m_audio_interface;
}

CPU::CPUManager& System::GetCPU() const
{
  return m_impl->m_cpu;
}

CoreTiming::CoreTimingManager& System::GetCoreTiming() const
{
  return m_impl->m_core_timing;
}

CommandProcessor::CommandProcessorManager& System::GetCommandProcessor() const
{
  return m_impl->m_command_processor;
}

DSP::DSPManager& System::GetDSP() const
{
  return m_impl->m_dsp;
}

DVDInterface::DVDInterfaceManager& System::GetDVDInterface() const
{
  return m_impl->m_dvd_interface;
}

DVDThread::DVDThreadState& System::GetDVDThreadState() const
{
  return m_impl->m_dvd_thread_state;
}

ExpansionInterface::ExpansionInterfaceState& System::GetExpansionInterfaceState() const
{
  return m_impl->m_expansion_interface_state;
}

Fifo::FifoManager& System::GetFifo() const
{
  return m_impl->m_fifo;
}

GeometryShaderManager& System::GetGeometryShaderManager() const
{
  return m_impl->m_geometry_shader_manager;
}

GPFifo::GPFifoManager& System::GetGPFifo() const
{
  return m_impl->m_gp_fifo;
}

HSP::HSPManager& System::GetHSP() const
{
  return m_impl->m_hsp;
}

IOS::HLE::USB::SkylanderPortal& System::GetSkylanderPortal() const
{
  return m_impl->m_skylander_portal;
}

Memory::MemoryManager& System::GetMemory() const
{
  return m_impl->m_memory;
}

MemoryInterface::MemoryInterfaceState& System::GetMemoryInterfaceState() const
{
  return m_impl->m_memory_interface_state;
}

PixelEngine::PixelEngineManager& System::GetPixelEngine() const
{
  return m_impl->m_pixel_engine;
}

PixelShaderManager& System::GetPixelShaderManager() const
{
  return m_impl->m_pixel_shader_manager;
}

PowerPC::PowerPCState& System::GetPPCState() const
{
  return m_impl->m_ppc_state;
}

ProcessorInterface::ProcessorInterfaceManager& System::GetProcessorInterface() const
{
  return m_impl->m_processor_interface;
}

SerialInterface::SerialInterfaceState& System::GetSerialInterfaceState() const
{
  return m_impl->m_serial_interface_state;
}

Sram& System::GetSRAM() const
{
  return m_impl->m_sram;
}

VertexShaderManager& System::GetVertexShaderManager() const
{
  return m_impl->m_vertex_shader_manager;
}

VideoInterface::VideoInterfaceState& System::GetVideoInterfaceState() const
{
  return m_impl->m_video_interface_state;
}
}  // namespace Core
