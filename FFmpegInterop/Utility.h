//*****************************************************************************
//
//	Copyright 2019 Microsoft Corporation
//
//	Licensed under the Apache License, Version 2.0 (the "License");
//	you may not use this file except in compliance with the License.
//	You may obtain a copy of the License at
//
//	http ://www.apache.org/licenses/LICENSE-2.0
//
//	Unless required by applicable law or agreed to in writing, software
//	distributed under the License is distributed on an "AS IS" BASIS,
//	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//	See the License for the specific language governing permissions and
//	limitations under the License.
//
//*****************************************************************************

#pragma once

namespace FFmpegInterop
{
	// Map of FFmpeg errors -> HRESULTs
	const std::map<int, HRESULT> ffmpegToHRESULTMap
	{
		{ AVERROR(EINVAL), E_INVALIDARG },
		{ AVERROR(ENOMEM), E_OUTOFMEMORY },
		{ AVERROR_BUFFER_TOO_SMALL, MF_E_BUFFERTOOSMALL }
	};

	// Helper function to map FFmpeg errors to HRESULTs
	inline HRESULT averror_to_hresult(_In_range_(< , 0) int result)
	{
		auto iter = ffmpegToHRESULTMap.find(result);
		return (iter != ffmpegToHRESULTMap.end()) ? iter->second : E_FAIL;
	}

	// Macro to check the result of FFmpeg calls
	#define THROW_IF_FFMPEG_FAILED(result) if ((result) < 0) { THROW_HR(averror_to_hresult(result)); }

	// Smart classes for managing FFmpeg objects
	struct AVCodecContextDeleter
	{
		void operator()(_In_opt_ AVCodecContext* codecContext)
		{
			avcodec_free_context(&codecContext);
		}
	};
	typedef std::unique_ptr<AVCodecContext, AVCodecContextDeleter> AVCodecContext_ptr;

	struct AVCodecParametersDeleter
	{
		void operator()(_In_opt_ AVCodecParameters* codecParams)
		{
			avcodec_parameters_free(&codecParams);
		}
	};
	typedef std::unique_ptr<AVCodecParameters, AVCodecParametersDeleter> AVCodecParameters_ptr;

	struct AVPacketDeleter
	{
		void operator()(_In_opt_ AVPacket* packet)
		{
			av_packet_free(&packet);
		}
	};
	typedef std::unique_ptr<AVPacket, AVPacketDeleter> AVPacket_ptr;

	struct AVFrameDeleter
	{
		void operator()(_In_opt_ AVFrame* frame)
		{
			av_frame_free(&frame);
		}
	};
	typedef std::unique_ptr<AVFrame, AVFrameDeleter> AVFrame_ptr;

	struct AVImage
	{
		AVImage() :
			data{ nullptr, nullptr, nullptr, nullptr },
			lineSizes{ -1, -1, -1, -1 }
		{

		}

		AVImage(_Inout_ AVImage&& other) noexcept
		{
			std::copy(std::begin(other.data), std::end(other.data), data);
			std::copy(std::begin(other.lineSizes), std::end(other.lineSizes), lineSizes);

			std::fill(std::begin(other.data), std::end(other.data), nullptr);
		}

		~AVImage()
		{
			av_freep(&data[0]);
		}

		AVImage& operator=(_Inout_ AVImage&& other) noexcept
		{
			if (this != &other)
			{
				std::swap(data, other.data);
				std::copy(std::begin(other.lineSizes), std::end(other.lineSizes), lineSizes);
			}
		}

		void Reset()
		{
			av_freep(&data[0]);
			std::fill(std::begin(data), std::end(data), nullptr);
			std::fill(std::begin(lineSizes), std::end(lineSizes), -1);
		}

		// These variables should always be set together and only by av_image_alloc()
		std::byte* data[4];
		int lineSizes[4];
	};

	struct AVSamples
	{
		AVSamples() = default;

		AVSamples(_Inout_ AVSamples&& other) noexcept :
			data(std::exchange(other.data, nullptr)),
			size(other.size)
		{

		}

		~AVSamples()
		{
			av_freep(&data);
		}

		AVSamples& operator=(_Inout_ AVSamples&& other) noexcept
		{
			if (this != &other)
			{
				std::swap(data, other.data);
			}
		}

		void Reset()
		{
			av_freep(&data);
			size = -1;
		}

		// These variables should always be set together and only by av_samples_alloc()
		std::byte* data{ nullptr };
		int size{ -1 };
	};

	struct SwrContextDeleter
	{
		void operator()(_In_opt_ SwrContext* swrContext)
		{
			swr_free(&swrContext);
		}
	};
	typedef std::unique_ptr<SwrContext, SwrContextDeleter> SwrContext_ptr;

	struct SwsContextDeleter
	{
		void operator()(_In_opt_ SwsContext* swsContext)
		{
			sws_freeContext(swsContext);
		}
	};
	typedef std::unique_ptr<SwsContext, SwsContextDeleter> SwsContext_ptr;
}
