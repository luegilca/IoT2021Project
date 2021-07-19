class UsagesController < ApplicationController
  before_action :set_device, only: [:reset_by_device, :usage_by_device]

  # GET /usages/:id
  def usage_by_device
    if @device
      if @device.usages.size > 0
        render json: @device, root: 'data', serializer: UsageCustomSerializer
      else
        render json: {data: {error: "Device ##{params[:id]} has not been used yet."}}, status: 500
      end
    else
      render json: {data: {error: "Device ##{params[:id]} cannot be found."}}, status: 404
    end
  end

  # POST /usages
  def create_for_device
    device = Device.find_by(device_id: usage_params[:device_id])
    if device
      usages = Usage.where(device: device).count
      used_volume = usages.to_f * device.dispensation_ml
      if (device.max_capacity_ml - used_volume) < device.alert_capacity_ml
        render json: {data: {error: "Device #{usage_params[:device_id]} is empty!"}}, status: 422
      else
        usage = Usage.create(device: device)
        if usage.save
          render json: {data: { message: "Usage registered on device #{usage_params[:device_id]}!" } }, status: 200
        else
          render json: {data: {error: "Cannot create register."}}, status: 422
        end
      end
    else
      render json: {data: {error: "Device ##{usage_params[:device_id]} cannot be found."}}, status: 404
    end
  end

  # DELETE /usages/:id
  def reset_by_device
    if @device
      if @device.usages.size > 0
        @device.usages.destroy_all
        render json: {data: {messsage: "Usages from device ##{params[:id]} has been deleted."}}, status: 200
      else
        render json: {data: {error: "Device ##{params[:id]} has not been used yet."}}, status: 500
      end
    else
      render json: {data: {error: "Device ##{params[:id]} cannot be found."}}, status: 404
    end
    
  end

  private
  def set_device
    @device = Device.find_by(device_id: params[:id])
  end

  def usage_params
    params.require(:device).permit(:device_id)
  end
end
