#pragma once

#include<memory>

namespace dxgame {

	typedef long long int64;

	class ITimer {
	public:
		virtual void setTargetFps(float fps) noexcept = 0;
		virtual float getFps() const noexcept = 0;
		virtual float getTime() const noexcept = 0;
		virtual float getRealTime() const noexcept = 0;
		virtual float getDeltaTime() const noexcept = 0;
		virtual bool init() noexcept = 0;
		virtual void beginFrame() noexcept = 0;
		virtual void endFrame() noexcept = 0;
		virtual void start() noexcept = 0;
		virtual void pause() noexcept = 0;
		virtual void stop() noexcept = 0;
	};

#ifdef WIN32
	class Win32Timer : public ITimer
	{
	public:
		explicit Win32Timer() noexcept;

		virtual void setTargetFps(float fps) noexcept override;
		virtual float getFps() const noexcept override;
		virtual float getTime() const noexcept override;
		virtual float getRealTime() const noexcept override;
		virtual float getDeltaTime() const noexcept override;
		virtual bool init() noexcept override;
		virtual void beginFrame() noexcept override;
		virtual void endFrame() noexcept override;
		virtual void start() noexcept override;
		virtual void pause() noexcept override;
		virtual void stop() noexcept override;

	private:
		int64 getRealCounter() noexcept;
		void calculateFps() noexcept;

		int64	m_BeginningCounter;
		int64	m_PrevCounter;
		int64	m_CurCounter;
		int64	m_TotalFrame;
		int64	m_PausedCounter;
		int64	m_RunningCounter;
		double  m_CountPerSecond;
		float	m_FramePerSecond;
		int64	m_OnFrameBeginningCounter;
		double  m_TargetFramePerSecond;
		bool	m_Paused;
	};
#endif

	class TimerService
	{
	private:
		static std::shared_ptr<ITimer> timer;

	public:
		static std::shared_ptr<ITimer> getTimer();
	};
}