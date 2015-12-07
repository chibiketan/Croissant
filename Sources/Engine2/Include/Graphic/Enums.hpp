#ifndef ENGINE_GRAPHIC_ENUMS_HPP
#define ENGINE_GRAPHIC_ENUMS_HPP

namespace Croissant
{
	namespace Graphic
	{
		enum class OpenGLValueNameEnum
		{
			ActiveTexture = 0,
			AliasedLineWidthRange,
			SmoothLineWidthRange,
			SmoothLineWidthGranularity,
			ArrayBufferBinding,
			Blend,
			BlendColor,
			BlendDstAlpha,
			BlendDstRgb,
			BlendEquationRgb,
			BlendEquationAlpha,
			BlendSrcAlpha,
			BlendSrcRgb,
			ColorClearValue,
			ColorLogicOp,
			ColorWritemask,
			CompressedTextureFormats,
			CullFace,
			CurrentProgram,
			DepthClearValue,
			DepthFunc,
			DepthRange,
			DepthTest,
			DepthWritemask,
			Dither,
			Doublebuffer,
			DrawBuffer,
			DrawBuffer0,
			DrawBuffer1,
			DrawBuffer2,
			DrawBuffer3,
			DrawBuffer4,
			DrawBuffer5,
			DrawBuffer6,
			DrawBuffer7,
			DrawBuffer8,
			DrawBuffer9,
			DrawBuffer10,
			DrawBuffer11,
			DrawBuffer12,
			DrawBuffer13,
			DrawBuffer14,
			DrawBuffer15,
			DrawFramebufferBinding,
			ReadFramebufferBinding,
			ElementArrayBufferBinding,
			RenderbufferBinding,
			FragmentShaderDerivativeHint,
			LineSmooth,
			LineSmoothHint,
			LineWidth,
			LogicOpMode,
			Max3DTextureSize,
			MaxClipDistances,
			MaxCombinedFragmentUniformComponents,
			MaxCombinedTextureImageUnits,
			MaxCombinedVertexUniformComponents,
			MaxCombinedGeometryUniformComponents,
			MaxVaryingComponents,
			MaxCombinedUniformBlocks,
			MaxCubeMapTextureSize,
			MaxDrawBuffers,
			MaxDualSourceDrawBuffers,
			MaxElementsIndices,
			MaxElementsVertices,
			MaxFragmentUniformComponents,
			MaxFragmentUniformBlocks,
			MaxFragmentInputComponents,
			MinProgramTexelOffset,
			MaxProgramTexelOffset,
			MaxRectangleTextureSize,
			MaxTextureImageUnits,
			MaxTextureLodBias,
			MaxTextureSize,
			MaxRenderbufferSize,
			MaxArrayTextureLayers,
			MaxTextureBufferSize,
			MaxUniformBlockSize,
			MaxVaryingFloats,
			MaxVertexAttribs,
			MaxVertexTextureImageUnits,
			MaxGeometryTextureImageUnits,
			MaxVertexUniformComponents,
			MaxVertexOutputComponents,
			MaxGeometryUniformComponents,
			MaxSamplMaskWords,
			MaxColorTextureSamples,
			MaxDepthTextureSamples,
			MaxIntegerSamples,
			MawServerWaitTimeout,
			MaxUniformBufferBindings,
			MaxVertexUniformBlocks,
			MaxGeometryUniformBlocks,
			MaxGeometryInputComponents,
			MaxGeometryOutputComponents,
			MaxViewportDims,
			NumCompressedTextureFormats,
			PackAlignment,
			PackImageHeight,
			PackLsbFirst,
			PackRowLength,
			PackSkipImages,
			PackSkipPixels,
			PackSkipRows,
			PackSwapBytes,
			PixelPackBufferBinding,
			PixelUnpackBufferBinding,
			PointFadeThresholdSize,
			PrimitiveRestartIndex,
			ProgramPointSize,
			ProvokingVertex,
			PointSize,
			PointSizeGranularity,
			PointSizeRange,
			PolygonOffsetFactor,
			PolygonOffsetUnits,
			PolygonOffsetFill,
			PolygonOffsetLine,
			PolygonOffsetPoint,
			PolygonSmooth,
			PolygonSmoothHint,
			ReadBuffer,
			SampleBuffers,
			SampleCoverageValue,
			SampleCoverageInvert,
			SamplerBinding,
			Samples,
			ScissorBox,
			ScissorTest,
			StencilBackFail,
			StencilBackFunc,
			StencilBackPassDepthFail,
			StencilBackPassDepthPass,
			StencilBackRef,
			StencilBackValueMask,
			StencilBackWritemask,
			StencilClearValue,
			StencilFail,
			StencilFunc,
			StencilPassDepthFail,
			StencilPassDepthPass,
			StencilRef,
			StencilTest,
			StencilValueMask,
			StencilWritemask,
			Stereo,
			SubpixelBits,
			TextureBinding1D,
			TextureBinding1DArray,
			TextureBinding2D,
			TextureBinding2DArray,
			TextureBinding2DMultisample,
			TextureBinding2DMultisampleArray,
			TextureBinding3D,
			TextureBindingBuffer,
			TextureBindingCubeMap,
			TextureBindingRectangle,
			TextureCompressionHint,
			Timestamp,
			TransformFeedbackBufferBinding,
			TransformFeedbackBufferStart,
			TransformFeedbackBufferSize,
			UniformBufferBinding,
			UniformBufferStart,
			UniformBufferSize,
			UniformBufferOffsetAlignment,
			UnpackAlignment,
			UnpackImageHeight,
			UnpackLsbFirst,
			UnpackRowLength,
			UnpackSkipImages,
			UnpackSkipPixels,
			UnpackSkipRows,
			UnpackSwapBytes,
			NumExtentions,
			MajorVersion,
			MinorVersion,
			ContextFlags,
			Viewport,

			MAX_ELEMENT = Viewport
		};

		enum class OpenGLShaderIntegerNameEnum
		{
			ShaderType,
			DeleteStatus,
			CompileStatus,
			InfoLogLength,
			ShaderSourceLength,

			MAX_ELEMENT = ShaderSourceLength
		};
	}
}

#endif // !ifndef ENGINE_GRAPHIC_ENUMS_HPP