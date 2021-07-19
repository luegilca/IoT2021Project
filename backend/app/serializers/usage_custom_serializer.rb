class UsageCustomSerializer < ActiveModel::Serializer
  type 'data'
  attributes :device_id, :max_capacity_ml, :dispensation_ml, :alert_capacity_ml, 
      :status, :latitude, :longitude, :current_volume, :first_usage, :last_usage,
      :low_level  

  def current_volume   
    usages = Usage.where(device: object).count
    used_volume = usages.to_f * object.dispensation_ml
    return object.max_capacity_ml - used_volume
  end

  def low_level
    usages = Usage.where(device: object).count
    used_volume = usages.to_f * object.dispensation_ml
    return (object.max_capacity_ml - used_volume) < object.alert_capacity_ml
  end

  def first_usage
    return Usage.where(device: object).order(created_at: :desc).first.created_at.strftime("%F %T")
  end 

  def last_usage
    return Usage.where(device: object).order(created_at: :asc).first.created_at.strftime("%F %T")
  end 
end
