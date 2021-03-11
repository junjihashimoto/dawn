// Copyright 2017 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef DAWNNATIVE_DEVICE_H_
#define DAWNNATIVE_DEVICE_H_

#include "dawn_native/Commands.h"
#include "dawn_native/Error.h"
#include "dawn_native/Extensions.h"
#include "dawn_native/Format.h"
#include "dawn_native/Forward.h"
#include "dawn_native/ObjectBase.h"
#include "dawn_native/Toggles.h"

#include "dawn_native/DawnNative.h"
#include "dawn_native/dawn_platform.h"

#include <memory>

namespace dawn_native {
    class AdapterBase;
    class AttachmentState;
    class AttachmentStateBlueprint;
    class BindGroupLayoutBase;
    class CreatePipelineAsyncTracker;
    class DynamicUploader;
    class ErrorScopeStack;
    class PersistentCache;
    class StagingBufferBase;
    struct InternalPipelineStore;
    struct ShaderModuleParseResult;

    class DeviceBase {
      public:
        DeviceBase(AdapterBase* adapter, const DeviceDescriptor* descriptor);
        virtual ~DeviceBase();

        void HandleError(InternalErrorType type, const char* message);

        bool ConsumedError(MaybeError maybeError) {
            if (DAWN_UNLIKELY(maybeError.IsError())) {
                ConsumeError(maybeError.AcquireError());
                return true;
            }
            return false;
        }

        template <typename T>
        bool ConsumedError(ResultOrError<T> resultOrError, T* result) {
            if (DAWN_UNLIKELY(resultOrError.IsError())) {
                ConsumeError(resultOrError.AcquireError());
                return true;
            }
            *result = resultOrError.AcquireSuccess();
            return false;
        }

        MaybeError ValidateObject(const ObjectBase* object) const;

        AdapterBase* GetAdapter() const;
        dawn_platform::Platform* GetPlatform() const;

        // Returns the Format corresponding to the wgpu::TextureFormat or an error if the format
        // isn't a valid wgpu::TextureFormat or isn't supported by this device.
        // The pointer returned has the same lifetime as the device.
        ResultOrError<const Format*> GetInternalFormat(wgpu::TextureFormat format) const;

        // Returns the Format corresponding to the wgpu::TextureFormat and assumes the format is
        // valid and supported.
        // The reference returned has the same lifetime as the device.
        const Format& GetValidInternalFormat(wgpu::TextureFormat format) const;

        virtual CommandBufferBase* CreateCommandBuffer(
            CommandEncoder* encoder,
            const CommandBufferDescriptor* descriptor) = 0;

        ExecutionSerial GetCompletedCommandSerial() const;
        ExecutionSerial GetLastSubmittedCommandSerial() const;
        ExecutionSerial GetFutureSerial() const;
        ExecutionSerial GetPendingCommandSerial() const;
        virtual MaybeError TickImpl() = 0;

        // Many Dawn objects are completely immutable once created which means that if two
        // creations are given the same arguments, they can return the same object. Reusing
        // objects will help make comparisons between objects by a single pointer comparison.
        //
        // Technically no object is immutable as they have a reference count, and an
        // application with reference-counting issues could "see" that objects are reused.
        // This is solved by automatic-reference counting, and also the fact that when using
        // the client-server wire every creation will get a different proxy object, with a
        // different reference count.
        //
        // When trying to create an object, we give both the descriptor and an example of what
        // the created object will be, the "blueprint". The blueprint is just a FooBase object
        // instead of a backend Foo object. If the blueprint doesn't match an object in the
        // cache, then the descriptor is used to make a new object.
        ResultOrError<Ref<BindGroupLayoutBase>> GetOrCreateBindGroupLayout(
            const BindGroupLayoutDescriptor* descriptor);
        void UncacheBindGroupLayout(BindGroupLayoutBase* obj);

        BindGroupLayoutBase* GetEmptyBindGroupLayout();

        ResultOrError<ComputePipelineBase*> GetOrCreateComputePipeline(
            const ComputePipelineDescriptor* descriptor);
        void UncacheComputePipeline(ComputePipelineBase* obj);

        ResultOrError<PipelineLayoutBase*> GetOrCreatePipelineLayout(
            const PipelineLayoutDescriptor* descriptor);
        void UncachePipelineLayout(PipelineLayoutBase* obj);

        ResultOrError<RenderPipelineBase*> GetOrCreateRenderPipeline(
            const RenderPipelineDescriptor* descriptor);
        void UncacheRenderPipeline(RenderPipelineBase* obj);

        ResultOrError<SamplerBase*> GetOrCreateSampler(const SamplerDescriptor* descriptor);
        void UncacheSampler(SamplerBase* obj);

        ResultOrError<ShaderModuleBase*> GetOrCreateShaderModule(
            const ShaderModuleDescriptor* descriptor,
            ShaderModuleParseResult* parseResult);
        void UncacheShaderModule(ShaderModuleBase* obj);

        Ref<AttachmentState> GetOrCreateAttachmentState(AttachmentStateBlueprint* blueprint);
        Ref<AttachmentState> GetOrCreateAttachmentState(
            const RenderBundleEncoderDescriptor* descriptor);
        Ref<AttachmentState> GetOrCreateAttachmentState(const RenderPipelineDescriptor* descriptor);
        Ref<AttachmentState> GetOrCreateAttachmentState(const RenderPassDescriptor* descriptor);
        void UncacheAttachmentState(AttachmentState* obj);

        // Dawn API
        BindGroupBase* CreateBindGroup(const BindGroupDescriptor* descriptor);
        BindGroupLayoutBase* CreateBindGroupLayout(const BindGroupLayoutDescriptor* descriptor);
        BufferBase* CreateBuffer(const BufferDescriptor* descriptor);
        CommandEncoder* CreateCommandEncoder(const CommandEncoderDescriptor* descriptor);
        ComputePipelineBase* CreateComputePipeline(const ComputePipelineDescriptor* descriptor);
        PipelineLayoutBase* CreatePipelineLayout(const PipelineLayoutDescriptor* descriptor);
        QuerySetBase* CreateQuerySet(const QuerySetDescriptor* descriptor);
        void CreateComputePipelineAsync(const ComputePipelineDescriptor* descriptor,
                                        WGPUCreateComputePipelineAsyncCallback callback,
                                        void* userdata);
        void CreateRenderPipelineAsync(const RenderPipelineDescriptor* descriptor,
                                       WGPUCreateRenderPipelineAsyncCallback callback,
                                       void* userdata);
        RenderBundleEncoder* CreateRenderBundleEncoder(
            const RenderBundleEncoderDescriptor* descriptor);
        RenderPipelineBase* CreateRenderPipeline(const RenderPipelineDescriptor* descriptor);
        RenderPipelineBase* CreateRenderPipeline2(const RenderPipelineDescriptor2* descriptor);
        SamplerBase* CreateSampler(const SamplerDescriptor* descriptor);
        ShaderModuleBase* CreateShaderModule(const ShaderModuleDescriptor* descriptor);
        SwapChainBase* CreateSwapChain(Surface* surface, const SwapChainDescriptor* descriptor);
        TextureBase* CreateTexture(const TextureDescriptor* descriptor);
        TextureViewBase* CreateTextureView(TextureBase* texture,
                                           const TextureViewDescriptor* descriptor);
        InternalPipelineStore* GetInternalPipelineStore();

        // For Dawn Wire
        BufferBase* CreateErrorBuffer();

        // TODO(dawn:22): Remove once the deprecation period is finished.
        QueueBase* GetDefaultQueue();
        QueueBase* GetQueue();

        void InjectError(wgpu::ErrorType type, const char* message);
        bool Tick();

        void SetDeviceLostCallback(wgpu::DeviceLostCallback callback, void* userdata);
        void SetUncapturedErrorCallback(wgpu::ErrorCallback callback, void* userdata);
        void PushErrorScope(wgpu::ErrorFilter filter);
        bool PopErrorScope(wgpu::ErrorCallback callback, void* userdata);

        MaybeError ValidateIsAlive() const;

        PersistentCache* GetPersistentCache();

        void Reference();
        void Release();

        virtual ResultOrError<std::unique_ptr<StagingBufferBase>> CreateStagingBuffer(
            size_t size) = 0;
        virtual MaybeError CopyFromStagingToBuffer(StagingBufferBase* source,
                                                   uint64_t sourceOffset,
                                                   BufferBase* destination,
                                                   uint64_t destinationOffset,
                                                   uint64_t size) = 0;
        virtual MaybeError CopyFromStagingToTexture(const StagingBufferBase* source,
                                                    const TextureDataLayout& src,
                                                    TextureCopy* dst,
                                                    const Extent3D& copySizePixels) = 0;

        DynamicUploader* GetDynamicUploader() const;

        // The device state which is a combination of creation state and loss state.
        //
        //   - BeingCreated: the device didn't finish creation yet and the frontend cannot be used
        //     (both for the application calling WebGPU, or re-entrant calls). No work exists on
        //     the GPU timeline.
        //   - Alive: the device is usable and might have work happening on the GPU timeline.
        //   - BeingDisconnected: the device is no longer usable because we are waiting for all
        //     work on the GPU timeline to finish. (this is to make validation prevent the
        //     application from adding more work during the transition from Available to
        //     Disconnected)
        //   - Disconnected: there is no longer work happening on the GPU timeline and the CPU data
        //     structures can be safely destroyed without additional synchronization.
        enum class State {
            BeingCreated,
            Alive,
            BeingDisconnected,
            Disconnected,
        };
        State GetState() const;
        bool IsLost() const;

        std::vector<const char*> GetEnabledExtensions() const;
        std::vector<const char*> GetTogglesUsed() const;
        bool IsExtensionEnabled(Extension extension) const;
        bool IsToggleEnabled(Toggle toggle) const;
        bool IsValidationEnabled() const;
        bool IsRobustnessEnabled() const;
        size_t GetLazyClearCountForTesting();
        void IncrementLazyClearCountForTesting();
        size_t GetDeprecationWarningCountForTesting();
        void EmitDeprecationWarning(const char* warning);
        void LoseForTesting();

        // AddFutureSerial is used to update the mFutureSerial with the max serial needed to be
        // ticked in order to clean up all pending callback work or to execute asynchronous resource
        // writes. It should be given the serial that a callback is tracked with, so that once that
        // serial is completed, it can be resolved and cleaned up. This is so that when there is no
        // gpu work (the last submitted serial has not moved beyond the completed serial), Tick can
        // still check if we have pending work to take care of, rather than hanging and never
        // reaching the serial the work will be executed on.
        void AddFutureSerial(ExecutionSerial serial);
        // Check for passed fences and set the new completed serial
        void CheckPassedSerials();

        virtual uint32_t GetOptimalBytesPerRowAlignment() const = 0;
        virtual uint64_t GetOptimalBufferToTextureCopyOffsetAlignment() const = 0;

        virtual float GetTimestampPeriodInNS() const = 0;

      protected:
        void SetToggle(Toggle toggle, bool isEnabled);
        void ForceSetToggle(Toggle toggle, bool isEnabled);

        MaybeError Initialize(QueueBase* defaultQueue);
        void ShutDownBase();

        // Incrememt mLastSubmittedSerial when we submit the next serial
        void IncrementLastSubmittedCommandSerial();

      private:
        virtual ResultOrError<BindGroupBase*> CreateBindGroupImpl(
            const BindGroupDescriptor* descriptor) = 0;
        virtual ResultOrError<BindGroupLayoutBase*> CreateBindGroupLayoutImpl(
            const BindGroupLayoutDescriptor* descriptor) = 0;
        virtual ResultOrError<Ref<BufferBase>> CreateBufferImpl(
            const BufferDescriptor* descriptor) = 0;
        virtual ResultOrError<ComputePipelineBase*> CreateComputePipelineImpl(
            const ComputePipelineDescriptor* descriptor) = 0;
        virtual ResultOrError<PipelineLayoutBase*> CreatePipelineLayoutImpl(
            const PipelineLayoutDescriptor* descriptor) = 0;
        virtual ResultOrError<QuerySetBase*> CreateQuerySetImpl(
            const QuerySetDescriptor* descriptor) = 0;
        virtual ResultOrError<RenderPipelineBase*> CreateRenderPipelineImpl(
            const RenderPipelineDescriptor* descriptor) = 0;
        virtual ResultOrError<SamplerBase*> CreateSamplerImpl(
            const SamplerDescriptor* descriptor) = 0;
        virtual ResultOrError<ShaderModuleBase*> CreateShaderModuleImpl(
            const ShaderModuleDescriptor* descriptor,
            ShaderModuleParseResult* parseResult) = 0;
        virtual ResultOrError<SwapChainBase*> CreateSwapChainImpl(
            const SwapChainDescriptor* descriptor) = 0;
        // Note that previousSwapChain may be nullptr, or come from a different backend.
        virtual ResultOrError<NewSwapChainBase*> CreateSwapChainImpl(
            Surface* surface,
            NewSwapChainBase* previousSwapChain,
            const SwapChainDescriptor* descriptor) = 0;
        virtual ResultOrError<Ref<TextureBase>> CreateTextureImpl(
            const TextureDescriptor* descriptor) = 0;
        virtual ResultOrError<TextureViewBase*> CreateTextureViewImpl(
            TextureBase* texture,
            const TextureViewDescriptor* descriptor) = 0;

        ResultOrError<Ref<BindGroupLayoutBase>> CreateEmptyBindGroupLayout();

        MaybeError CreateBindGroupInternal(BindGroupBase** result,
                                           const BindGroupDescriptor* descriptor);
        MaybeError CreateBindGroupLayoutInternal(BindGroupLayoutBase** result,
                                                 const BindGroupLayoutDescriptor* descriptor);
        ResultOrError<Ref<BufferBase>> CreateBufferInternal(const BufferDescriptor* descriptor);
        MaybeError CreateComputePipelineInternal(ComputePipelineBase** result,
                                                 const ComputePipelineDescriptor* descriptor);
        MaybeError CreatePipelineLayoutInternal(PipelineLayoutBase** result,
                                                const PipelineLayoutDescriptor* descriptor);
        MaybeError CreateQuerySetInternal(QuerySetBase** result,
                                          const QuerySetDescriptor* descriptor);
        MaybeError CreateRenderBundleEncoderInternal(
            RenderBundleEncoder** result,
            const RenderBundleEncoderDescriptor* descriptor);
        MaybeError CreateRenderPipelineInternal(RenderPipelineBase** result,
                                                const RenderPipelineDescriptor2* descriptor);
        MaybeError CreateRenderPipelineInternal(RenderPipelineBase** result,
                                                const RenderPipelineDescriptor* descriptor);
        MaybeError CreateSamplerInternal(SamplerBase** result, const SamplerDescriptor* descriptor);
        MaybeError CreateShaderModuleInternal(ShaderModuleBase** result,
                                              const ShaderModuleDescriptor* descriptor);
        MaybeError CreateSwapChainInternal(SwapChainBase** result,
                                           Surface* surface,
                                           const SwapChainDescriptor* descriptor);
        ResultOrError<Ref<TextureBase>> CreateTextureInternal(const TextureDescriptor* descriptor);
        MaybeError CreateTextureViewInternal(TextureViewBase** result,
                                             TextureBase* texture,
                                             const TextureViewDescriptor* descriptor);

        void ApplyToggleOverrides(const DeviceDescriptor* deviceDescriptor);
        void ApplyExtensions(const DeviceDescriptor* deviceDescriptor);

        void SetDefaultToggles();

        void ConsumeError(std::unique_ptr<ErrorData> error);

        // Each backend should implement to check their passed fences if there are any and return a
        // completed serial. Return 0 should indicate no fences to check.
        virtual ExecutionSerial CheckAndUpdateCompletedSerials() = 0;
        // During shut down of device, some operations might have been started since the last submit
        // and waiting on a serial that doesn't have a corresponding fence enqueued. Fake serials to
        // make all commands look completed.
        void AssumeCommandsComplete();
        bool IsDeviceIdle();

        // mCompletedSerial tracks the last completed command serial that the fence has returned.
        // mLastSubmittedSerial tracks the last submitted command serial.
        // During device removal, the serials could be artificially incremented
        // to make it appear as if commands have been compeleted. They can also be artificially
        // incremented when no work is being done in the GPU so CPU operations don't have to wait on
        // stale serials.
        // mFutureSerial tracks the largest serial we need to tick to for asynchronous commands or
        // callbacks to fire
        ExecutionSerial mCompletedSerial = ExecutionSerial(0);
        ExecutionSerial mLastSubmittedSerial = ExecutionSerial(0);
        ExecutionSerial mFutureSerial = ExecutionSerial(0);

        // ShutDownImpl is used to clean up and release resources used by device, does not wait for
        // GPU or check errors.
        virtual void ShutDownImpl() = 0;

        // WaitForIdleForDestruction waits for GPU to finish, checks errors and gets ready for
        // destruction. This is only used when properly destructing the device. For a real
        // device loss, this function doesn't need to be called since the driver already closed all
        // resources.
        virtual MaybeError WaitForIdleForDestruction() = 0;

        wgpu::ErrorCallback mUncapturedErrorCallback = nullptr;
        void* mUncapturedErrorUserdata = nullptr;

        wgpu::DeviceLostCallback mDeviceLostCallback = nullptr;
        void* mDeviceLostUserdata = nullptr;

        std::unique_ptr<ErrorScopeStack> mErrorScopeStack;

        // The Device keeps a ref to the Instance so that any live Device keeps the Instance alive.
        // The Instance shouldn't need to ref child objects so this shouldn't introduce ref cycles.
        // The Device keeps a simple pointer to the Adapter because the Adapter is owned by the
        // Instance.
        Ref<InstanceBase> mInstance;
        AdapterBase* mAdapter = nullptr;

        // The object caches aren't exposed in the header as they would require a lot of
        // additional includes.
        struct Caches;
        std::unique_ptr<Caches> mCaches;

        Ref<BindGroupLayoutBase> mEmptyBindGroupLayout;

        std::unique_ptr<DynamicUploader> mDynamicUploader;
        std::unique_ptr<CreatePipelineAsyncTracker> mCreatePipelineAsyncTracker;
        Ref<QueueBase> mQueue;

        struct DeprecationWarnings;
        std::unique_ptr<DeprecationWarnings> mDeprecationWarnings;

        uint32_t mRefCount = 1;
        State mState = State::BeingCreated;

        FormatTable mFormatTable;

        TogglesSet mEnabledToggles;
        TogglesSet mOverridenToggles;
        size_t mLazyClearCountForTesting = 0;

        ExtensionsSet mEnabledExtensions;

        std::unique_ptr<InternalPipelineStore> mInternalPipelineStore;

        std::unique_ptr<PersistentCache> mPersistentCache;
    };

}  // namespace dawn_native

#endif  // DAWNNATIVE_DEVICE_H_
